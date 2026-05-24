import csv
import os
import shutil
import struct
import sys
import time
import xml.etree.ElementTree as ET
from pathlib import Path


WORKSPACE = Path(r"C:\0-NewDmo")
NEW_LIST_XML = WORKSPACE / "Dcommerce" / "xml" / "AllBins" / "Data" / "DigimonListData.xml"
NEW_EVO_XML = WORKSPACE / "Dcommerce" / "xml" / "AllBins" / "Data" / "DEvolutionList.xml"
OLD_BIN_DIR = WORKSPACE / "dmo-server-main-postgres" / "Bins" / "data" / "bin" / "english"
OLD_LIST_BIN = OLD_BIN_DIR / "Digimon_List.bin"
OLD_EVO_BIN = OLD_BIN_DIR / "DigimonEvo.bin"
OUT_DIR = WORKSPACE / "DigimonBinMigrationFeature" / "generated" / "converted"
REPORT_DIR = WORKSPACE / "DigimonBinMigrationFeature" / "generated" / "reports"

DIGIMON_LIST_RECORD_SIZE = 572
EVOLVE_OBJ_SIZE = 328
MAX_CLIENT_EVO_SLOT = 16


def scalar(node, default=0):
    if node is None:
        return default
    value = node.get("value")
    if value in (None, ""):
        return default
    if "." in value:
        return float(value)
    return int(value)


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


def read_legacy_digimon_list(path):
    data = path.read_bytes()
    count = struct.unpack_from("<I", data, 0)[0]
    expected = 4 + count * DIGIMON_LIST_RECORD_SIZE
    if len(data) != expected:
        raise ValueError(f"{path} has {len(data)} bytes, expected {expected}")

    ids = []
    names = {}
    for index in range(count):
        off = 4 + index * DIGIMON_LIST_RECORD_SIZE
        digimon_id = struct.unpack_from("<I", data, off)[0]
        name_raw = data[off + 8 : off + 136]
        name = name_raw.decode("utf-16le", errors="ignore").split("\x00", 1)[0]
        ids.append(digimon_id)
        names[digimon_id] = name
    return ids, names


def read_legacy_evo(path):
    data = path.read_bytes()
    off = 0
    count = struct.unpack_from("<I", data, off)[0]
    off += 4
    bases = {}
    for _ in range(count):
        start = off
        base_id, battle_type = struct.unpack_from("<II", data, off)
        off += 8
        evo_count = struct.unpack_from("<i", data, off)[0]
        off += 4
        lines = []
        for _line in range(evo_count):
            rec = data[off : off + EVOLVE_OBJ_SIZE]
            target_id = struct.unpack_from("<I", rec, 0)[0]
            evo_slot = struct.unpack_from("<H", rec, 4)[0]
            lines.append((target_id, evo_slot))
            off += EVOLVE_OBJ_SIZE
        bases[base_id] = {
            "battle_type": battle_type,
            "lines": lines,
            "raw": data[start:off],
        }
    tail = data[off:]
    return bases, tail


def parse_array_u16(parent, tag):
    node = parent.find(tag)
    if node is None:
        return []
    result = []
    for child in node:
        result.append(scalar(child))
    return result


def parse_skills(parent):
    node = parent.find("Skills")
    slots = {}
    if node is None:
        return slots
    for child in node:
        slot = scalar(child.find("SkillSlot"))
        skill_id = scalar(child.find("SkillId"))
        req_prev = scalar(child.find("RequiredPreviousSkillLevel"))
        slots[slot] = (skill_id, req_prev)
    return slots


def parse_digimon_list_xml(path):
    rows = []
    root = ET.parse(path).getroot()
    section = root.find("Digimon")
    if section is None:
        raise ValueError("Digimon section not found")

    for rec in section:
        if rec.get("kind") != "Record":
            continue
        family_types = parse_array_u16(rec, "FamilyTypes")
        nature_types = parse_array_u16(rec, "BaseNatureTypes")
        rows.append(
            {
                "digimon_id": scalar(rec.find("DigimonId")),
                "table_key": scalar(rec.find("DigimonTableKey")),
                "model_id": scalar(rec.find("ModelId")),
                "name": text(rec.find("Name")),
                "effect_path": text(rec.find("EffectPath")),
                "select_scale": float(scalar(rec.find("SelectScale"), 0)),
                "base_level": scalar(rec.find("BaseLevel")),
                "grow_type": scalar(rec.find("GrowType")),
                "field_p": scalar(rec.find("FieldP")),
                "char_size": scalar(rec.find("CharSize")),
                "evolution_type": scalar(rec.find("EvolutionType")),
                "attribute_type": scalar(rec.find("AttributeType")),
                "family_types": family_types,
                "base_nature_type": scalar(rec.find("BaseNatureType")),
                "base_nature_types": nature_types,
                "hp": scalar(rec.find("HP")),
                "ds": scalar(rec.find("DS")),
                "de": scalar(rec.find("DE")),
                "ev": scalar(rec.find("EV")),
                "ms": scalar(rec.find("MS")),
                "cr": scalar(rec.find("CR")),
                "at": scalar(rec.find("AT")),
                "as": scalar(rec.find("AS")),
                "ar": scalar(rec.find("AR")),
                "ht": scalar(rec.find("HT")),
                "skills": parse_skills(rec),
                "walk_len": float(scalar(rec.find("WalkLen"), 0)),
                "run_len": float(scalar(rec.find("RunLen"), 0)),
                "arun_len": float(scalar(rec.find("ARunLen"), 0)),
                "rank": scalar(rec.find("DigimonRank")),
            }
        )
    return rows


def get_struct_child(parent, tag):
    node = parent.find(tag)
    if node is None:
        return None
    return node


def parse_targets(parent, tag):
    node = parent.find(tag)
    result = []
    if node is None:
        return result
    if tag == "BaseTarget":
        return [
            {
                "slot": scalar(node.find("Slot")),
                "type": scalar(node.find("Type")),
                "digimon_id": scalar(node.find("DigimonId")),
            }
        ]
    for child in node:
        result.append(
            {
                "slot": scalar(child.find("Slot")),
                "type": scalar(child.find("Type")),
                "digimon_id": scalar(child.find("DigimonId")),
            }
        )
    return result


def parse_evolution_xml(path):
    rows = []
    root = ET.parse(path).getroot()
    section = root.find("DigimonEvolution")
    if section is None:
        raise ValueError("DigimonEvolution section not found")

    for rec in section:
        if rec.get("kind") != "Record":
            continue

        evolutions = []
        evols_node = rec.find("Evolutions")
        if evols_node is not None:
            for evo in evols_node:
                slot_open = get_struct_child(evo, "SlotOpenLimit")
                card = get_struct_child(evo, "CardInfo")
                render = get_struct_child(evo, "RenderInfo")
                effect = get_struct_child(evo, "EffectPos")
                icon = get_struct_child(evo, "UiIconPos")
                jogress = get_struct_child(evo, "JogressInfo")

                need_ids = []
                need_node = None if jogress is None else jogress.find("NeedDigimonIds")
                if need_node is not None:
                    for child in need_node:
                        need_ids.append(scalar(child))

                evo_slot = scalar(evo.find("EvoSlot"))
                if evo_slot > MAX_CLIENT_EVO_SLOT:
                    continue

                targets = [
                    target
                    for target in parse_targets(evo, "Targets")
                    if target["slot"] <= MAX_CLIENT_EVO_SLOT
                ]

                evolutions.append(
                    {
                        "evo_slot": evo_slot,
                        "target_id": scalar(evo.find("TargetDigimonId")),
                        "evo_type": scalar(evo.find("EvoType")),
                        "targets": targets,
                        "base_target": parse_targets(evo, "BaseTarget")[0],
                        "enable_slot": scalar(evo.find("EnableSlot")),
                        "open_qualification": scalar(slot_open.find("OpenQualification")) if slot_open is not None else 0,
                        "open_level": scalar(slot_open.find("OpenLevel")) if slot_open is not None else 0,
                        "open_quest": scalar(slot_open.find("OpenQuest")) if slot_open is not None else 0,
                        "open_item_type": scalar(slot_open.find("OpenItemType")) if slot_open is not None else 0,
                        "open_item_num": scalar(slot_open.find("OpenItemNum")) if slot_open is not None else 0,
                        "use_item": scalar(slot_open.find("UseItem")) if slot_open is not None else 0,
                        "use_item_num": scalar(slot_open.find("UseItemNum")) if slot_open is not None else 0,
                        "intimacy": scalar(slot_open.find("Intimacy")) if slot_open is not None else 0,
                        "open_crest": scalar(slot_open.find("OpenCrest")) if slot_open is not None else 0,
                        "evo_card1": scalar(card.find("EvoCard1")) if card is not None else 0,
                        "evo_card2": scalar(card.find("EvoCard2")) if card is not None else 0,
                        "evo_card3": scalar(card.find("EvoCard3")) if card is not None else 0,
                        "evo_digimental": scalar(card.find("EvoDigimental")) if card is not None else 0,
                        "evo_tamer_ds": scalar(evo.find("EvoTamerDs")),
                        "render": scalar(render.find("Render")) if render is not None else 0,
                        "target_start_x": scalar(render.find("TargetStartPosX")) if render is not None else 0,
                        "target_start_y": scalar(render.find("TargetStartPosY")) if render is not None else 0,
                        "start_height": scalar(render.find("StartHeight")) if render is not None else 0,
                        "start_rot": scalar(render.find("StartRot")) if render is not None else 0,
                        "target_end_x": scalar(render.find("TargetEndPosX")) if render is not None else 0,
                        "target_end_y": scalar(render.find("TargetEndPosY")) if render is not None else 0,
                        "end_height": scalar(render.find("EndHeight")) if render is not None else 0,
                        "end_rot": scalar(render.find("EndRot")) if render is not None else 0,
                        "speed": scalar(render.find("Speed")) if render is not None else 0,
                        "animation_id": scalar(render.find("AnimationId")) if render is not None else 0,
                        "start_time": float(scalar(render.find("StartTime"), 0)) if render is not None else 0.0,
                        "end_time": float(scalar(render.find("EndTime"), 0)) if render is not None else 0.0,
                        "background_r": scalar(render.find("BackgroundR")) if render is not None else 0,
                        "background_g": scalar(render.find("BackgroundG")) if render is not None else 0,
                        "background_b": scalar(render.find("BackgroundB")) if render is not None else 0,
                        "level_bone": text(effect.find("LevelBone")) if effect is not None else "",
                        "enchant_bone": text(effect.find("EnchantBone")) if effect is not None else "",
                        "size_bone": text(effect.find("SizeBone")) if effect is not None else "",
                        "icon_x": scalar(icon.find("PosX")) if icon is not None else 0,
                        "icon_y": scalar(icon.find("PosY")) if icon is not None else 0,
                        "capsule_evo_tree": scalar(evo.find("CapsuleEvoTree")),
                        "jogress_quest_check": scalar(jogress.find("JogressQuestCheck")) if jogress is not None else 0,
                        "chipset_type": scalar(jogress.find("ChipsetType")) if jogress is not None else 0,
                        "chipset_type_consume": scalar(jogress.find("ChipsetTypeConsume")) if jogress is not None else 0,
                        "chipset_num": scalar(jogress.find("ChipsetNum")) if jogress is not None else 0,
                        "chipset_type_period": scalar(jogress.find("ChipsetTypePeriod")) if jogress is not None else 0,
                        "need_digimon_ids": need_ids,
                    }
                )
        rows.append({"base_id": scalar(rec.find("DigimonId")), "evolutions": evolutions})
    return rows


def u16(value, field, warnings):
    if value < 0 or value > 0xFFFF:
        warnings.append((field, value, "truncated-to-u16"))
        return value & 0xFFFF
    return value


def i32(value, field, warnings):
    if 0x80000000 <= value <= 0xFFFFFFFF:
        return value - 0x100000000
    if value < -0x80000000 or value > 0x7FFFFFFF:
        warnings.append((field, value, "truncated-to-i32"))
        return 0
    return value


def write_digimon_record(row, warnings):
    family = (row["family_types"] + [0, 0, 0])[:3]
    natures = (row["base_nature_types"] + [0, 0, 0])[:3]
    skills = []
    for slot in range(4):
        skills.append(row["skills"].get(slot, (0, 0)))

    rec = bytearray(DIGIMON_LIST_RECORD_SIZE)
    struct.pack_into("<II", rec, 0, row["digimon_id"], row["model_id"])
    rec[8:136] = fixed_wide(row["name"], 128)
    rec[136:200] = fixed_ascii("", 64)
    struct.pack_into("<f", rec, 200, row["select_scale"])
    rec[204:332] = fixed_wide(row["effect_path"], 128)
    struct.pack_into(
        "<iiiiiiii",
        rec,
        332,
        i32(row["evolution_type"], "EvolutionType", warnings),
        i32(row["attribute_type"], "AttributeType", warnings),
        i32(family[0], "FamilyType[0]", warnings),
        i32(family[1], "FamilyType[1]", warnings),
        i32(family[2], "FamilyType[2]", warnings),
        i32(row["base_nature_type"], "BaseNatureType", warnings),
        i32(natures[0], "BaseNatureTypes[0]", warnings),
        i32(natures[1], "BaseNatureTypes[1]", warnings),
    )
    struct.pack_into("<i", rec, 364, i32(natures[2], "BaseNatureTypes[2]", warnings))
    struct.pack_into("<I", rec, 368, row["base_level"])
    struct.pack_into(
        "<HHHHHHHHHH",
        rec,
        372,
        u16(row["hp"], "HP", warnings),
        u16(row["ds"], "DS", warnings),
        u16(row["de"], "DE", warnings),
        u16(row["ev"], "EV", warnings),
        u16(row["ms"], "MS", warnings),
        u16(row["cr"], "CR", warnings),
        u16(row["at"], "AT", warnings),
        u16(row["as"], "AS", warnings),
        u16(row["ar"], "AR", warnings),
        u16(row["ht"], "HT", warnings),
    )
    rec[392] = u16(row["grow_type"], "GrowType", warnings) & 0xFF
    rec[393] = 0
    struct.pack_into("<H", rec, 394, u16(row["char_size"], "CharSize", warnings))
    for index, (skill_id, req_prev) in enumerate(skills):
        struct.pack_into(
            "<Ii",
            rec,
            396 + index * 8,
            skill_id & 0xFFFFFFFF,
            i32(req_prev, "RequiredPreviousSkillLevel", warnings),
        )
    struct.pack_into("<fff", rec, 428, row["walk_len"], row["run_len"], row["arun_len"])
    rec[440:568] = fixed_wide("", 128)
    struct.pack_into("<i", rec, 568, i32(row["rank"], "DigimonRank", warnings))
    return bytes(rec)


def write_evolve_obj(evo, warnings):
    rec = bytearray(EVOLVE_OBJ_SIZE)
    struct.pack_into("<IHH", rec, 0, evo["target_id"], u16(evo["evo_slot"], "EvoSlot", warnings), 0)

    # Legacy quick evolution reserves UI index 8 for devolution. Modern Global
    # stores that relation as BaseTarget/DEvolReturn data instead of a normal
    # outgoing target, so materialize it back into m_nEvolutionList[8].
    targets = evo["targets"][:8]
    base_target = evo.get("base_target") or {"slot": 0, "type": 0, "digimon_id": 0}
    if evo["target_id"] != base_target.get("digimon_id", 0) and base_target.get("digimon_id", 0) > 0:
        while len(targets) < 8:
            targets.append({"slot": 0, "type": 0, "digimon_id": 0})
        targets.append(
            {
                "slot": base_target["slot"],
                "type": base_target["type"],
                "digimon_id": base_target["digimon_id"],
            }
        )

    for index in range(9):
        target = targets[index] if index < len(targets) else {"slot": 0, "type": 0, "digimon_id": 0}
        struct.pack_into(
            "<HHI",
            rec,
            8 + index * 8,
            u16(target["slot"], "EvolutionList.Slot", warnings),
            u16(target["type"], "EvolutionList.Type", warnings),
            target["digimon_id"] & 0xFFFFFFFF,
        )

    struct.pack_into("<ii", rec, 80, i32(evo["icon_x"], "IconPos.X", warnings), i32(evo["icon_y"], "IconPos.Y", warnings))

    # Legacy client order is EnableSlot then OpenQualification, despite one schema copy listing
    # the two names reversed.
    struct.pack_into(
        "<HHHHHHHHHHHHHHHH",
        rec,
        88,
        u16(evo["enable_slot"], "EnableSlot", warnings),
        u16(evo["open_qualification"], "OpenQualification", warnings),
        u16(evo["open_level"], "OpenLevel", warnings),
        u16(evo["open_quest"], "OpenQuest", warnings),
        u16(evo["open_item_type"], "OpenItemType", warnings),
        u16(evo["open_item_num"], "OpenItemNum", warnings),
        u16(evo["use_item"], "UseItem", warnings),
        u16(evo["use_item_num"], "UseItemNum", warnings),
        u16(evo["intimacy"], "Intimacy", warnings),
        u16(evo["open_crest"], "OpenCrest", warnings),
        u16(evo["evo_card1"], "EvoCard1", warnings),
        u16(evo["evo_card2"], "EvoCard2", warnings),
        u16(evo["evo_card3"], "EvoCard3", warnings),
        u16(evo["evo_digimental"], "EvoDigimental", warnings),
        u16(evo["evo_tamer_ds"], "EvoTamerDs", warnings),
        0,
    )

    struct.pack_into("<i", rec, 120, i32(evo["render"], "Render", warnings))
    struct.pack_into(
        "<iiiiiiiiII",
        rec,
        124,
        i32(evo["target_start_x"], "TargetStartPosX", warnings),
        i32(evo["target_start_y"], "TargetStartPosY", warnings),
        i32(evo["start_height"], "StartHeight", warnings),
        i32(evo["start_rot"], "StartRot", warnings),
        i32(evo["target_end_x"], "TargetEndPosX", warnings),
        i32(evo["target_end_y"], "TargetEndPosY", warnings),
        i32(evo["end_height"], "EndHeight", warnings),
        i32(evo["end_rot"], "EndRot", warnings),
        evo["speed"] & 0xFFFFFFFF,
        evo["animation_id"] & 0xFFFFFFFF,
    )
    struct.pack_into("<I", rec, 164, 0)
    struct.pack_into("<dd", rec, 168, evo["start_time"], evo["end_time"])
    struct.pack_into(
        "<iii",
        rec,
        184,
        i32(evo["background_r"], "BackgroundR", warnings),
        i32(evo["background_g"], "BackgroundG", warnings),
        i32(evo["background_b"], "BackgroundB", warnings),
    )
    rec[196:228] = fixed_ascii(evo["level_bone"], 32)
    rec[228:260] = fixed_ascii(evo["enchant_bone"], 32)
    rec[260:292] = fixed_ascii(evo["size_bone"], 32)
    struct.pack_into("<iI", rec, 292, i32(evo["capsule_evo_tree"], "CapsuleEvoTree", warnings), evo["jogress_quest_check"] & 0xFFFFFFFF)
    struct.pack_into(
        "<HHHHHH",
        rec,
        300,
        u16(evo["chipset_type"], "ChipsetType", warnings),
        u16(evo["chipset_type_consume"], "ChipsetTypeConsume", warnings),
        u16(evo["chipset_num"], "ChipsetNum", warnings),
        u16(evo["chipset_type_period"], "ChipsetTypePeriod", warnings),
        u16(len(evo["need_digimon_ids"]), "NeedDigimonCount", warnings),
        0,
    )
    for index in range(3):
        value = evo["need_digimon_ids"][index] if index < len(evo["need_digimon_ids"]) else 0
        struct.pack_into("<i", rec, 312 + index * 4, i32(value, "NeedDigimonIds", warnings))
    struct.pack_into("<I", rec, 324, 0)
    return bytes(rec)


def write_outputs(digimon_rows, evo_rows, old_battle_tail):
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    REPORT_DIR.mkdir(parents=True, exist_ok=True)

    warnings = []
    list_bytes = bytearray()
    list_bytes += struct.pack("<I", len(digimon_rows))
    for row in digimon_rows:
        list_bytes += write_digimon_record(row, warnings)

    evo_bytes = bytearray()
    evo_bytes += struct.pack("<I", len(evo_rows))
    for row in evo_rows:
        evo_bytes += struct.pack("<IIi", row["base_id"], 0, len(row["evolutions"]))
        for evo in row["evolutions"]:
            evo_bytes += write_evolve_obj(evo, warnings)
    evo_bytes += old_battle_tail

    (OUT_DIR / "Digimon_List.bin").write_bytes(list_bytes)
    (OUT_DIR / "DigimonEvo.bin").write_bytes(evo_bytes)
    return warnings


def report_csv(path, fieldnames, rows):
    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)


def main():
    old_ids, old_names = read_legacy_digimon_list(OLD_LIST_BIN)
    old_evos, old_battle_tail = read_legacy_evo(OLD_EVO_BIN)
    digimon_rows = parse_digimon_list_xml(NEW_LIST_XML)
    evo_rows = parse_evolution_xml(NEW_EVO_XML)

    digimon_rows.sort(key=lambda row: row["digimon_id"])
    evo_rows.sort(key=lambda row: row["base_id"])

    warnings = write_outputs(digimon_rows, evo_rows, old_battle_tail)

    new_by_id = {row["digimon_id"]: row for row in digimon_rows}
    new_ids = set(new_by_id)
    old_id_set = set(old_ids)

    report_csv(
        REPORT_DIR / "new-digimon-list.csv",
        ["DigimonId", "Name", "ModelId", "EvolutionType", "Rank"],
        [
            {
                "DigimonId": row["digimon_id"],
                "Name": row["name"],
                "ModelId": row["model_id"],
                "EvolutionType": row["evolution_type"],
                "Rank": row["rank"],
            }
            for row in digimon_rows
            if row["digimon_id"] not in old_id_set
        ],
    )

    old_base_set = set(old_evos)
    new_base_rows = []
    changed_base_rows = []
    for row in evo_rows:
        base_id = row["base_id"]
        new_lines = {(evo["target_id"], evo["evo_slot"]) for evo in row["evolutions"]}
        old_lines = set(old_evos.get(base_id, {}).get("lines", []))
        added = sorted(new_lines - old_lines, key=lambda x: (x[1], x[0]))
        if base_id not in old_base_set:
            new_base_rows.append(
                {
                    "BaseId": base_id,
                    "BaseName": new_by_id.get(base_id, {}).get("name", ""),
                    "EvolutionCount": len(row["evolutions"]),
                }
            )
        elif added:
            changed_base_rows.append(
                {
                    "BaseId": base_id,
                    "BaseName": new_by_id.get(base_id, {}).get("name", ""),
                    "AddedEvolutionCount": len(added),
                    "AddedEvolutions": "; ".join(
                        f"{slot}:{target}:{new_by_id.get(target, {}).get('name', '')}" for target, slot in added
                    ),
                }
            )

    report_csv(REPORT_DIR / "new-evolution-bases.csv", ["BaseId", "BaseName", "EvolutionCount"], new_base_rows)
    report_csv(
        REPORT_DIR / "changed-existing-evolution-bases.csv",
        ["BaseId", "BaseName", "AddedEvolutionCount", "AddedEvolutions"],
        changed_base_rows,
    )
    report_csv(
        REPORT_DIR / "conversion-warnings.csv",
        ["Field", "Value", "Action"],
        [{"Field": f, "Value": v, "Action": a} for f, v, a in warnings],
    )

    out_list = OUT_DIR / "Digimon_List.bin"
    out_evo = OUT_DIR / "DigimonEvo.bin"
    print(f"Generated {out_list} ({out_list.stat().st_size} bytes, {len(digimon_rows)} rows)")
    print(f"Generated {out_evo} ({out_evo.stat().st_size} bytes, {len(evo_rows)} trees)")
    print(f"New digimon rows: {len(new_ids - old_id_set)}")
    print(f"New evolution bases: {len(new_base_rows)}")
    print(f"Changed existing evolution bases: {len(changed_base_rows)}")
    print(f"Warnings: {len(warnings)}")


if __name__ == "__main__":
    main()
