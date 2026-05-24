from __future__ import annotations

import argparse
import csv
import struct
from dataclasses import dataclass
from pathlib import Path


LEGACY_INFO_SIZE = 196
SEQUENCE_INFO_SIZE = 16
LEGACY_EVENT_SIZE = 192
GLOBAL_EVENT_SIZE = 200
SHADER_SIZE = 68


@dataclass
class SequenceRecord:
    info: bytes
    events: list[bytes]
    shaders: list[bytes]


@dataclass
class ModelRecord:
    model_id: int
    info: bytes
    path: str
    sequences: list[SequenceRecord]


def c_string(data: bytes) -> str:
    end = data.find(b"\0")
    if end >= 0:
        data = data[:end]
    return data.decode("ascii", "replace")


def parse_model_dat(path: Path, event_size: int) -> list[ModelRecord]:
    data = path.read_bytes()
    if len(data) < 4:
        raise ValueError(f"{path} is too small")

    count = struct.unpack_from("<i", data, 0)[0]
    offset = 4
    records: list[ModelRecord] = []

    for index in range(count):
        if offset + LEGACY_INFO_SIZE > len(data):
            raise ValueError(f"{path}: row {index} info exceeds EOF at {offset}")

        info = data[offset : offset + LEGACY_INFO_SIZE]
        model_id = struct.unpack_from("<I", info, 0)[0]
        model_path = c_string(info[4:164])
        sequence_count = struct.unpack_from("<i", info, 176)[0]
        offset += LEGACY_INFO_SIZE

        if sequence_count < 0 or sequence_count > 1000:
            raise ValueError(
                f"{path}: row {index} id={model_id} has invalid sequence count "
                f"{sequence_count} at {offset}"
            )

        sequences: list[SequenceRecord] = []
        for seq_index in range(sequence_count):
            if offset + SEQUENCE_INFO_SIZE > len(data):
                raise ValueError(f"{path}: row {index} seq {seq_index} exceeds EOF at {offset}")

            seq_info = data[offset : offset + SEQUENCE_INFO_SIZE]
            _seq_id, event_count, _loop_count, shader_count = struct.unpack("<Iiii", seq_info)
            offset += SEQUENCE_INFO_SIZE

            if event_count < 0 or event_count > 1000 or shader_count < 0 or shader_count > 1000:
                raise ValueError(
                    f"{path}: row {index} id={model_id} seq={seq_index} has invalid counts "
                    f"events={event_count} shaders={shader_count} at {offset}"
                )

            event_bytes = event_count * event_size
            shader_bytes = shader_count * SHADER_SIZE
            if offset + event_bytes + shader_bytes > len(data):
                raise ValueError(
                    f"{path}: row {index} id={model_id} seq={seq_index} exceeds EOF at {offset}"
                )

            events = [
                data[offset + i * event_size : offset + (i + 1) * event_size]
                for i in range(event_count)
            ]
            offset += event_bytes

            shaders = [
                data[offset + i * SHADER_SIZE : offset + (i + 1) * SHADER_SIZE]
                for i in range(shader_count)
            ]
            offset += shader_bytes

            sequences.append(SequenceRecord(seq_info, events, shaders))

        records.append(ModelRecord(model_id, info, model_path, sequences))

    if offset != len(data):
        raise ValueError(f"{path}: parsed {offset} bytes but file has {len(data)} bytes")

    return records


def write_legacy_model_dat(path: Path, records: list[ModelRecord], source_event_size: dict[int, int]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("wb") as file:
        file.write(struct.pack("<i", len(records)))
        for record in records:
            file.write(record.info)
            for sequence in record.sequences:
                file.write(sequence.info)
                for event in sequence.events:
                    event_size = source_event_size[record.model_id]
                    if event_size == LEGACY_EVENT_SIZE:
                        file.write(event)
                    elif event_size == GLOBAL_EVENT_SIZE:
                        file.write(event[:LEGACY_EVENT_SIZE])
                    else:
                        raise ValueError(f"unsupported event size {event_size} for {record.model_id}")
                for shader in sequence.shaders:
                    file.write(shader)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Merge Global Model.dat entries into the legacy LK-DMO Model.dat layout."
    )
    parser.add_argument("--legacy", required=True, type=Path)
    parser.add_argument("--global", dest="global_path", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--report", required=True, type=Path)
    args = parser.parse_args()

    legacy_records = parse_model_dat(args.legacy, LEGACY_EVENT_SIZE)
    global_records = parse_model_dat(args.global_path, GLOBAL_EVENT_SIZE)

    legacy_by_id = {record.model_id: record for record in legacy_records}
    global_by_id = {record.model_id: record for record in global_records}

    merged = list(legacy_records)
    event_size_by_id = {record.model_id: LEGACY_EVENT_SIZE for record in legacy_records}

    added: list[ModelRecord] = []
    for record in global_records:
        if record.model_id in legacy_by_id:
            continue
        merged.append(record)
        added.append(record)
        event_size_by_id[record.model_id] = GLOBAL_EVENT_SIZE

    write_legacy_model_dat(args.output, merged, event_size_by_id)

    # Validate the produced file with the exact legacy layout.
    parsed_output = parse_model_dat(args.output, LEGACY_EVENT_SIZE)
    if len(parsed_output) != len(merged):
        raise ValueError("output validation count mismatch")

    args.report.parent.mkdir(parents=True, exist_ok=True)
    with args.report.open("w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)
        writer.writerow(["ModelId", "Path", "SequenceCount", "EventCount", "ShaderCount"])
        for record in added:
            event_count = sum(len(sequence.events) for sequence in record.sequences)
            shader_count = sum(len(sequence.shaders) for sequence in record.sequences)
            writer.writerow([record.model_id, record.path, len(record.sequences), event_count, shader_count])

    print(f"legacy records: {len(legacy_records)}")
    print(f"global records: {len(global_records)}")
    print(f"added records: {len(added)}")
    print(f"output records: {len(parsed_output)}")
    print(f"output: {args.output}")
    print(f"report: {args.report}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
