import argparse
import csv
import struct
import xml.etree.ElementTree as ET
from pathlib import Path


WORKSPACE = Path(r"C:\0-NewDmo")
DEFAULT_BASE_BUFF_BIN = WORKSPACE / "dmo-server-main-postgres" / "Bins" / "data" / "bin" / "english" / "Buff.bin"
DEFAULT_BUFF_DATA_XML = WORKSPACE / "Dcommerce" / "xml" / "AllBins" / "Data" / "Buff_Data.xml"
DEFAULT_SKILL_CODE_DIGIMON_XML = WORKSPACE / "Dcommerce" / "xml" / "AllBins" / "Data" / "SkillCodeDigimon.xml"
DEFAULT_BUFF_TEXT_XML = WORKSPACE / "TamerBinMigrationFeature" / "source_xml_language" / "_str" / "Buff_Str.xml"
DEFAULT_OUTPUT = WORKSPACE / "DigimonBinMigrationFeature" / "generated" / "converted" / "Buff.bin"
DEFAULT_REPORT = WORKSPACE / "DigimonBinMigrationFeature" / "generated" / "reports" / "digimon-skill-buffs.csv"

LEGACY_RECORD_SIZE = 476


def scalar(node, default=0):
    if node is None:
        return default
    value = node.get("value")
    if value in (None, ""):
        return default
    return int(float(value))


def text(node, default=""):
    if node is None or node.text is None:
        return default
    return node.text


def fixed_wide(value, byte_len):
    raw = value.encode("utf-16le", errors="replace")
    max_payload = byte_len - 2
    if len(raw) > max_payload:
        raw = raw[:max_payload]
        if len(raw) % 2:
            raw = raw[:-1]
    return raw + b"\x00" * (byte_len - len(raw))


def fixed_ascii(value, byte_len):
    raw = value.encode("ascii", errors="ignore")
    if len(raw) > byte_len - 1:
        raw = raw[: byte_len - 1]
    return raw + b"\x00" * (byte_len - len(raw))


def decode_fixed_wide(data):
    text_value = data.decode("utf-16le", errors="ignore")
    return text_value.split("\x00", 1)[0]


def read_text_map(path):
    if not path.exists():
        return {}

    root = ET.parse(path).getroot()
    section = root.find("Entries")
    if section is None:
        return {}

    result = {}
    for rec in section:
        if rec.get("kind") != "Record":
            continue
        key = scalar(rec.find("TextKey"))
        if key <= 0:
            continue
        result[key] = {
            "name": text(rec.find("TextA")),
            "comment": text(rec.find("TextB")),
        }
    return result


def apply_text_to_record(record, buff_id, text_map, only_placeholder=False):
    if buff_id not in text_map:
        return False

    current_name = decode_fixed_wide(record[2 : 2 + 128])
    if only_placeholder and current_name and current_name != f"Buff {buff_id}":
        return False

    row = text_map[buff_id]
    record[2 : 2 + 128] = fixed_wide(row["name"], 128)
    record[130 : 130 + 256] = fixed_wide(row["comment"], 256)
    return True


def read_legacy_buff_bin(path):
    data = path.read_bytes()
    count = struct.unpack_from("<I", data, 0)[0]
    expected = 4 + count * LEGACY_RECORD_SIZE
    if len(data) != expected:
        raise ValueError(f"{path} has {len(data)} bytes, expected {expected}")

    records = {}
    for index in range(count):
        offset = 4 + index * LEGACY_RECORD_SIZE
        record = data[offset : offset + LEGACY_RECORD_SIZE]
        buff_id = struct.unpack_from("<H", record, 0)[0]
        records[buff_id] = record
    return records


def read_digimon_skill_ids(path):
    root = ET.parse(path).getroot()
    section = root.find("Skills")
    if section is None:
        raise ValueError("SkillCodeDigimon.xml has no Skills section")

    result = set()
    for rec in section:
        if rec.get("kind") != "Record":
            continue
        skill_id = scalar(rec.find("SkillId"))
        if skill_id:
            result.add(skill_id)
    return result


def parse_global_digimon_buffs(path, digimon_skill_ids):
    root = ET.parse(path).getroot()
    section = root.find("Buffs")
    if section is None:
        raise ValueError("Buff_Data.xml has no Buffs section")

    rows = []
    for rec in section:
        if rec.get("kind") != "Record":
            continue

        skill_code = scalar(rec.find("SkillCode"))
        digimon_skill_code = scalar(rec.find("DigimonSkillCode"))
        if skill_code not in digimon_skill_ids and digimon_skill_code not in digimon_skill_ids:
            continue

        buff_id = scalar(rec.find("BuffId"))
        if buff_id <= 0:
            continue
        if buff_id > 0xFFFF:
            raise ValueError(f"BuffId {buff_id} does not fit legacy USHORT Buff.bin layout")

        rows.append(
            {
                "buff_id": buff_id,
                "info_id": scalar(rec.find("InfoId")),
                "icon": scalar(rec.find("BuffIcon")),
                "type": scalar(rec.find("BuffType")),
                "life_type": scalar(rec.find("BuffLifeType")),
                "time_type": scalar(rec.find("BuffTimeType")),
                "min_level": scalar(rec.find("MinLevel")),
                "class": scalar(rec.find("BuffClass")),
                "skill_code": skill_code,
                "digimon_skill_code": digimon_skill_code,
                "delete_on_condition": scalar(rec.find("DeleteOnCondition")),
                "effect_file": text(rec.find("ResourceName")),
                "condition_level": scalar(rec.find("ConditionLevel")),
            }
        )

    return rows


def write_legacy_record(row, previous=None, text_map=None):
    record = bytearray(LEGACY_RECORD_SIZE)

    if previous is not None:
        # Preserve legacy name/comment text where the buff already existed.
        record[2 : 2 + 128] = previous[2 : 2 + 128]
        record[130 : 130 + 256] = previous[130 : 130 + 256]
    else:
        record[2 : 2 + 128] = fixed_wide(f"Buff {row['buff_id']}", 128)
        record[130 : 130 + 256] = fixed_wide(row["effect_file"], 256)

    if text_map is not None:
        apply_text_to_record(record, row["buff_id"], text_map, only_placeholder=previous is not None)

    struct.pack_into("<H", record, 0, row["buff_id"])
    struct.pack_into("<H", record, 386, row["icon"])
    struct.pack_into("<H", record, 388, row["type"])
    struct.pack_into("<H", record, 390, row["life_type"])
    struct.pack_into("<H", record, 392, row["time_type"])
    struct.pack_into("<H", record, 394, row["min_level"])
    struct.pack_into("<H", record, 396, row["class"])
    # 398..399 is VC++ alignment padding before DWORD fields.
    struct.pack_into("<I", record, 400, row["skill_code"])
    struct.pack_into("<I", record, 404, row["digimon_skill_code"])
    struct.pack_into("<B", record, 408, row["delete_on_condition"])
    record[409 : 409 + 64] = fixed_ascii(row["effect_file"], 64)
    # 473 is padding before the final USHORT.
    struct.pack_into("<H", record, 474, row["condition_level"])
    return bytes(record)


def main():
    parser = argparse.ArgumentParser(
        description="Merge global Buff_Data records used by Digimon skills into the legacy Buff.bin layout."
    )
    parser.add_argument("--base-buff-bin", type=Path, default=DEFAULT_BASE_BUFF_BIN)
    parser.add_argument("--buff-data-xml", type=Path, default=DEFAULT_BUFF_DATA_XML)
    parser.add_argument("--skill-code-digimon-xml", type=Path, default=DEFAULT_SKILL_CODE_DIGIMON_XML)
    parser.add_argument("--buff-text-xml", type=Path, default=DEFAULT_BUFF_TEXT_XML)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    parser.add_argument("--replace-existing", action="store_true")
    args = parser.parse_args()

    records = read_legacy_buff_bin(args.base_buff_bin)
    base_count = len(records)
    text_map = read_text_map(args.buff_text_xml)
    digimon_skill_ids = read_digimon_skill_ids(args.skill_code_digimon_xml)
    rows = parse_global_digimon_buffs(args.buff_data_xml, digimon_skill_ids)

    added = []
    replaced = []
    skipped = []
    for row in rows:
        buff_id = row["buff_id"]
        previous = records.get(buff_id)
        if previous is not None and not args.replace_existing:
            skipped.append(row)
            continue

        records[buff_id] = write_legacy_record(row, previous, text_map)
        if previous is None:
            added.append(row)
        else:
            replaced.append(row)

    patched_names = []
    for buff_id, record in list(records.items()):
        mutable = bytearray(record)
        if apply_text_to_record(mutable, buff_id, text_map, only_placeholder=True):
            records[buff_id] = bytes(mutable)
            patched_names.append(buff_id)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("wb") as out:
        out.write(struct.pack("<I", len(records)))
        for buff_id in sorted(records):
            out.write(records[buff_id])

    args.report.parent.mkdir(parents=True, exist_ok=True)
    with args.report.open("w", newline="", encoding="utf-8") as fp:
        fieldnames = [
            "action",
            "buff_id",
            "icon",
            "type",
            "life_type",
            "time_type",
            "class",
            "skill_code",
            "digimon_skill_code",
            "delete_on_condition",
            "effect_file",
            "condition_level",
        ]
        writer = csv.DictWriter(fp, fieldnames=fieldnames)
        writer.writeheader()
        for action, group in (("added", added), ("replaced", replaced), ("skipped", skipped)):
            for row in group:
                writer.writerow({"action": action, **{k: row[k] for k in fieldnames if k != "action"}})

    print(
        {
            "Output": str(args.output),
            "Report": str(args.report),
            "BaseBuffCount": base_count,
            "BuffTextCount": len(text_map),
            "DigimonSkillIds": len(digimon_skill_ids),
            "GlobalDigimonBuffRows": len(rows),
            "Added": len(added),
            "Replaced": len(replaced),
            "SkippedExisting": len(skipped),
            "PatchedPlaceholderNames": len(patched_names),
            "FinalBuffCount": len(records),
            "Size": args.output.stat().st_size,
        }
    )


if __name__ == "__main__":
    main()
