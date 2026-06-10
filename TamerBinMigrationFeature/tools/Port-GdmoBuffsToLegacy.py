from __future__ import annotations

import argparse
import csv
import importlib.util
import shutil
import struct
import sys
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path


LEGACY_RECORD_SIZE = 476
BUFF_NAME_LEN = 64
BUFF_COMMENT_LEN = 128
BUFF_EFFECT_LEN = 64


@dataclass
class BuffRecord:
    buff_id: int
    name: str
    comment: str
    icon: int
    buff_type: int
    life_type: int
    time_type: int
    min_level: int
    buff_class: int
    skill_code: int
    digimon_skill_code: int
    deleted: bool
    effect_file: str
    condition_level: int
    source: str


def read_u32(data: bytes, offset: int) -> int:
    return struct.unpack_from("<I", data, offset)[0]


def read_wstring(data: bytes, offset: int) -> tuple[str, int]:
    length = read_u32(data, offset)
    offset += 4
    raw = data[offset : offset + (length * 2)]
    offset += length * 2
    return raw.decode("utf-16le", errors="replace").rstrip("\x00"), offset


def write_fixed_utf16(text: str, chars: int) -> bytes:
    encoded = text[: chars - 1].encode("utf-16le", errors="replace")
    return encoded + (b"\x00" * ((chars * 2) - len(encoded)))


def write_fixed_ansi(text: str, size: int) -> bytes:
    encoded = text[: size - 1].encode("ascii", errors="ignore")
    return encoded + (b"\x00" * (size - len(encoded)))


def load_pack03_decoder(decoder_path: Path):
    spec = importlib.util.spec_from_file_location("pack03_unpack_v6", str(decoder_path))
    if spec is None or spec.loader is None:
        raise RuntimeError(f"Could not load decoder: {decoder_path}")

    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def decode_modern_bin(decoder, exe_blob: bytes, path: Path) -> bytes:
    return decoder.decode_payload_runtime_chain(
        path.read_bytes(),
        callback_control=0x6C,
        strict_bit3=False,
        exe_blob=exe_blob,
    )


def parse_gdmo_buff_strings(data: bytes) -> dict[int, tuple[str, str]]:
    count = read_u32(data, 0)
    offset = 4
    rows: dict[int, tuple[str, str]] = {}
    for _ in range(count):
        buff_id = read_u32(data, offset)
        offset += 4
        name, offset = read_wstring(data, offset)
        comment, offset = read_wstring(data, offset)
        rows[buff_id] = (name, comment)

    if offset != len(data):
        raise ValueError(f"Buff_Str cursor mismatch: {offset}/{len(data)}")

    return rows


def parse_gdmo_buff_data(data: bytes, strings: dict[int, tuple[str, str]]) -> dict[int, BuffRecord]:
    count = read_u32(data, 0)
    offset = 4
    rows: dict[int, BuffRecord] = {}
    for _ in range(count):
        buff_id, duplicate_id = struct.unpack_from("<II", data, offset)
        offset += 8
        (
            icon,
            buff_type,
            life_type,
            time_type,
            min_level,
            buff_class,
            skill_code,
            digimon_skill_code,
            deleted,
        ) = struct.unpack_from("<6HII B", data, offset)
        offset += struct.calcsize("<6HII B")

        effect_len = read_u32(data, offset)
        offset += 4
        effect_file = data[offset : offset + effect_len].decode("ascii", errors="ignore").rstrip("\x00")
        offset += effect_len
        condition_level = struct.unpack_from("<H", data, offset)[0]
        offset += 2

        if buff_id != duplicate_id:
            raise ValueError(f"Buff_Data duplicate id mismatch: {buff_id}/{duplicate_id}")
        if buff_id > 0xFFFF:
            continue

        name, comment = strings.get(buff_id, ("", ""))
        rows[buff_id] = BuffRecord(
            buff_id=buff_id,
            name=name,
            comment=comment,
            icon=icon,
            buff_type=buff_type,
            life_type=life_type,
            time_type=time_type,
            min_level=min_level,
            buff_class=buff_class,
            skill_code=skill_code,
            digimon_skill_code=digimon_skill_code,
            deleted=deleted != 0,
            effect_file=effect_file,
            condition_level=condition_level,
            source="gdmo",
        )

    if offset != len(data):
        raise ValueError(f"Buff_Data cursor mismatch: {offset}/{len(data)}")

    return rows


def parse_legacy_buff_bin(path: Path) -> dict[int, BuffRecord]:
    data = path.read_bytes()
    count = read_u32(data, 0)
    expected = 4 + (count * LEGACY_RECORD_SIZE)
    if expected != len(data):
        raise ValueError(f"Legacy Buff.bin size mismatch: expected {expected}, got {len(data)}")

    rows: dict[int, BuffRecord] = {}
    offset = 4
    for _ in range(count):
        start = offset
        buff_id = struct.unpack_from("<H", data, offset)[0]
        offset += 2
        name = data[offset : offset + (BUFF_NAME_LEN * 2)].decode("utf-16le", errors="replace").rstrip("\x00")
        offset += BUFF_NAME_LEN * 2
        comment = data[offset : offset + (BUFF_COMMENT_LEN * 2)].decode("utf-16le", errors="replace").rstrip("\x00")
        offset += BUFF_COMMENT_LEN * 2
        icon, buff_type, life_type, time_type, min_level, buff_class = struct.unpack_from("<6H", data, offset)
        offset += 12
        offset += 2
        skill_code, digimon_skill_code = struct.unpack_from("<II", data, offset)
        offset += 8
        deleted = data[offset] != 0
        offset += 1
        effect_file = data[offset : offset + BUFF_EFFECT_LEN].decode("ascii", errors="ignore").rstrip("\x00")
        offset += BUFF_EFFECT_LEN
        offset += 1
        condition_level = struct.unpack_from("<H", data, offset)[0]
        offset += 2

        if offset - start != LEGACY_RECORD_SIZE:
            raise ValueError(f"Legacy record size mismatch at {buff_id}: {offset - start}")

        rows[buff_id] = BuffRecord(
            buff_id=buff_id,
            name=name,
            comment=comment,
            icon=icon,
            buff_type=buff_type,
            life_type=life_type,
            time_type=time_type,
            min_level=min_level,
            buff_class=buff_class,
            skill_code=skill_code,
            digimon_skill_code=digimon_skill_code,
            deleted=deleted,
            effect_file=effect_file,
            condition_level=condition_level,
            source="legacy",
        )

    return rows


def serialize_record(record: BuffRecord) -> bytes:
    parts = [
        struct.pack("<H", record.buff_id),
        write_fixed_utf16(record.name, BUFF_NAME_LEN),
        write_fixed_utf16(record.comment, BUFF_COMMENT_LEN),
        struct.pack(
            "<6H",
            record.icon,
            record.buff_type,
            record.life_type,
            record.time_type,
            record.min_level,
            record.buff_class,
        ),
        b"\x00\x00",
        struct.pack("<II", record.skill_code, record.digimon_skill_code),
        b"\x01" if record.deleted else b"\x00",
        write_fixed_ansi(record.effect_file, BUFF_EFFECT_LEN),
        b"\x00",
        struct.pack("<H", record.condition_level),
    ]
    blob = b"".join(parts)
    if len(blob) != LEGACY_RECORD_SIZE:
        raise ValueError(f"Serialized record size mismatch for {record.buff_id}: {len(blob)}")
    return blob


def serialize_legacy_buff_bin(records: dict[int, BuffRecord]) -> bytes:
    ordered = [records[key] for key in sorted(records)]
    return struct.pack("<I", len(ordered)) + b"".join(serialize_record(record) for record in ordered)


def pack_hash(path: str) -> int:
    result = 5381
    for ch in path.lower():
        if ch in ".\\":
            continue
        result = ((result << 5) + result + ord(ch)) & 0xFFFFFFFF
    return result


def patch_pack03(pack_base: Path, relative_path: str, source_file: Path, stamp: str) -> tuple[Path, Path]:
    if pack_base.suffix.lower() in (".hf", ".pf"):
        pack_base = pack_base.with_suffix("")
    hf_path = pack_base.with_suffix(".hf")
    pf_path = pack_base.with_suffix(".pf")
    if not hf_path.exists() or not pf_path.exists():
        raise FileNotFoundError(f"Pack03 pair not found at {pack_base}")

    hf_backup = hf_path.with_name(hf_path.name + f".bak_gdmo_buffs_{stamp}")
    pf_backup = pf_path.with_name(pf_path.name + f".bak_gdmo_buffs_{stamp}")
    shutil.copy2(hf_path, hf_backup)
    shutil.copy2(pf_path, pf_backup)

    hf = hf_path.read_bytes()
    version, count = struct.unpack_from("<II", hf, 0)
    if version != 0x10:
        raise ValueError(f"Unsupported Pack03 hf version 0x{version:08X}: {hf_path}")

    entries: dict[int, tuple[int, int, int, int, int]] = {}
    offset = 8
    for _ in range(count):
        plag, data_size, alloc_size, key, data_offset = struct.unpack_from("<III IQ", hf, offset)
        entries[key] = (plag, data_size, alloc_size, key, data_offset)
        offset += 24

    data = source_file.read_bytes()
    encoded_name = relative_path.encode("ascii")
    header = bytearray(260)
    for index, value in enumerate(encoded_name):
        header[index] = value ^ 0xD0
    chunk = bytes(header) + struct.pack("<II", 0, len(encoded_name)) + data

    with pf_path.open("ab") as pf:
        data_offset = pf.tell() + 268
        pf.write(chunk)

    key = pack_hash(relative_path)
    entries[key] = (1, len(data), len(data), key, data_offset)

    with hf_path.open("wb") as hf_out:
        hf_out.write(struct.pack("<II", 0x10, len(entries)))
        for entry in sorted(entries.values(), key=lambda item: item[3]):
            hf_out.write(struct.pack("<III IQ", *entry))

    return pf_backup, hf_backup


def copy_with_backup(source: Path, destination: Path, stamp: str) -> Path:
    if destination.exists():
        backup = destination.with_name(destination.name + f".bak_gdmo_buffs_{stamp}")
        shutil.copy2(destination, backup)
    else:
        backup = destination.with_name(destination.name + f".bak_gdmo_buffs_{stamp}.missing")
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, destination)
    return backup


def write_report(path: Path, records: dict[int, BuffRecord], legacy_ids: set[int], gdmo_ids: set[int]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.writer(handle)
        writer.writerow(
            [
                "buff_id",
                "source",
                "name",
                "icon",
                "type",
                "life_type",
                "time_type",
                "class",
                "skill_code",
                "digimon_skill_code",
                "deleted",
                "effect_file",
            ]
        )
        for buff_id in sorted(records):
            record = records[buff_id]
            if buff_id in gdmo_ids and buff_id in legacy_ids:
                source = "gdmo_replaced_legacy"
            elif buff_id in gdmo_ids:
                source = "gdmo_added"
            else:
                source = "legacy_preserved"
            writer.writerow(
                [
                    record.buff_id,
                    source,
                    record.name,
                    record.icon,
                    record.buff_type,
                    record.life_type,
                    record.time_type,
                    record.buff_class,
                    record.skill_code,
                    record.digimon_skill_code,
                    int(record.deleted),
                    record.effect_file,
                ]
            )


def main() -> int:
    parser = argparse.ArgumentParser(description="Port GDMO Pack03 Buff_Data/Buff_Str rows to v487 legacy Buff.bin.")
    parser.add_argument("--decoder", type=Path, default=Path(r"C:\0-NewDmo\new bins\pack03_unpack_v6.py"))
    parser.add_argument("--exe", type=Path, default=Path(r"C:\0-NewDmo\new bins\unpacked_GDMO.exe"))
    parser.add_argument("--gdmo-buff-data", type=Path, default=Path(r"C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\notused\Buff_Data.bin"))
    parser.add_argument("--gdmo-buff-str", type=Path, default=Path(r"C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\notused\Buff_Str.bin"))
    parser.add_argument("--legacy-buff", type=Path, default=Path(r"C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Buff.bin"))
    parser.add_argument("--out-dir", type=Path, default=Path(r"C:\0-NewDmo\TamerBinMigrationFeature\generated\gdmo-buffs"))
    parser.add_argument("--apply", action="store_true")
    parser.add_argument("--no-server", action="store_true")
    parser.add_argument("--server-buff", type=Path, default=Path(r"C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Buff.bin"))
    parser.add_argument("--pack03", type=Path, action="append", default=None)
    args = parser.parse_args()
    if args.pack03 is None:
        args.pack03 = [
            Path(r"C:\0-NewDmo\ClientDist1_x64\Data\Pack03"),
            Path(r"C:\0-NewDmo\ClientDist1\Data\Pack03"),
        ]

    stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    decoder = load_pack03_decoder(args.decoder)
    exe_blob = args.exe.read_bytes()
    gdmo_strings = parse_gdmo_buff_strings(decode_modern_bin(decoder, exe_blob, args.gdmo_buff_str))
    gdmo_records = parse_gdmo_buff_data(decode_modern_bin(decoder, exe_blob, args.gdmo_buff_data), gdmo_strings)
    legacy_records = parse_legacy_buff_bin(args.legacy_buff)

    legacy_ids = set(legacy_records)
    gdmo_ids = set(gdmo_records)
    merged = dict(legacy_records)
    merged.update(gdmo_records)

    args.out_dir.mkdir(parents=True, exist_ok=True)
    output = args.out_dir / "Buff.bin"
    output.write_bytes(serialize_legacy_buff_bin(merged))
    report = args.out_dir / "gdmo-buff-port-report.csv"
    write_report(report, merged, legacy_ids, gdmo_ids)

    print(f"legacy_count={len(legacy_records)}")
    print(f"gdmo_count={len(gdmo_records)}")
    print(f"replaced={len(legacy_ids & gdmo_ids)}")
    print(f"added={len(gdmo_ids - legacy_ids)}")
    print(f"preserved_legacy_only={len(legacy_ids - gdmo_ids)}")
    print(f"merged_count={len(merged)}")
    print(f"output={output}")
    print(f"report={report}")

    set_skill_ids = [2702279, 2702280, 2702281, 2702282, 2702283, 2702284, 2702826]
    by_skill = {record.skill_code: record for record in merged.values() if record.skill_code in set_skill_ids}
    for skill_code in set_skill_ids:
        record = by_skill.get(skill_code)
        if record:
            print(f"set_skill {skill_code} -> buff {record.buff_id} icon {record.icon} name {record.name}")
        else:
            print(f"set_skill {skill_code} -> missing")

    if args.apply:
        if not args.no_server:
            backup = copy_with_backup(output, args.server_buff, stamp)
            print(f"server_backup={backup}")
        for pack in args.pack03:
            pf_backup, hf_backup = patch_pack03(pack, r"data\bin\english\Buff.bin", output, stamp)
            print(f"pack03={pack} pf_backup={pf_backup} hf_backup={hf_backup}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
