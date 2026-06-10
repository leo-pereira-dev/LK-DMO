using System.IO;
using System.Text;
using System.Xml.Linq;
using LkDmoBinConverter.Models;

namespace LkDmoBinConverter.Services;

public sealed class MonsterConverter
{
    private const string MonsterRelativePath = @"data\bin\english\Monster.bin";
    private const string MapMonsterRelativePath = @"data\bin\english\MapMonsterList.bin";
    private const int MonsterRecordSize = 396;
    private const int MonsterSkillRecordSize = 144;
    private const int MonsterSkillTermsRecordSize = 12;
    private const int MapMonsterRecordSize = 48;
    private const int DefaultRespawnSeconds = 10;

    public Task<MonsterConversionReport> ConvertMonstersAsync(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        return Task.Run(() => ConvertMonsters(options, applyToPack, cancellationToken), cancellationToken);
    }

    private static MonsterConversionReport ConvertMonsters(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        cancellationToken.ThrowIfCancellationRequested();
        Directory.CreateDirectory(options.OutputDirectory);

        var report = new MonsterConversionReport
        {
            OutputDirectory = options.OutputDirectory,
            Success = false
        };

        var sources = ResolveSources(options.GdmoBinPath);
        report.SourceFiles.Add(sources.MonsterXml);
        report.SourceFiles.Add(sources.MonsterSkillXml);
        report.SourceFiles.Add(sources.MonsterSkillTermsXml);
        report.SourceFiles.Add(sources.MapMonsterXml);
        report.SourceFiles.Add(sources.MonsterNameXml);

        var oldMonster = ParseLegacyMonster(LoadCurrentMonster(options.LkdmoInputPath));
        var names = ParseMonsterNames(sources.MonsterNameXml);
        var monsters = ParseGdmoMonsters(sources.MonsterXml);
        var skills = ParseGdmoSkills(sources.MonsterSkillXml, report);
        var terms = ParseGdmoSkillTerms(sources.MonsterSkillTermsXml);
        var mapSpawns = ParseGdmoMapMonster(sources.MapMonsterXml);

        report.OldMonsterCount = oldMonster.NamesByMonsterId.Count;
        report.OldHitCount = oldMonster.HitRows.Count;
        report.GdmoMonsterCount = monsters.Count;
        report.GdmoSkillCount = skills.Count;
        report.GdmoTermsCount = terms.Count;
        report.GdmoMapCount = mapSpawns.Select(x => x.MapId).Distinct().Count();
        report.GdmoMapSpawnCount = mapSpawns.Count;
        report.OldOnlyMonsterCount = oldMonster.NamesByMonsterId.Keys.Count(id => !monsters.ContainsKey(id));

        var monsterOutput = BuildMonsterBin(monsters, oldMonster, names, skills, terms, report);
        var mapMonsterOutput = BuildMapMonsterListBin(mapSpawns, monsters.Keys.ToHashSet(), report);

        var monsterPath = Path.Combine(options.OutputDirectory, "Monster.bin");
        var mapMonsterPath = Path.Combine(options.OutputDirectory, "MapMonsterList.bin");
        File.WriteAllBytes(monsterPath, monsterOutput);
        File.WriteAllBytes(mapMonsterPath, mapMonsterOutput);

        report.MonsterOutputPath = monsterPath;
        report.MapMonsterOutputPath = mapMonsterPath;
        report.PatchedFiles.Add(monsterPath);
        report.PatchedFiles.Add(mapMonsterPath);

        if (report.OldOnlyMonsterCount > 0)
            report.Warnings.Add($"{report.OldOnlyMonsterCount} old LKDMO monsters are not present in the GDMO source and were not emitted.");

        if (report.NameFallbackCount > 0)
            report.Warnings.Add($"{report.NameFallbackCount} monsters had no GDMO MonsterName_Str entry; old name or ID fallback was used.");

        if (report.ExpClampedCount > 0)
            report.Warnings.Add($"{report.ExpClampedCount} GDMO EXP values exceeded the LKDMO UInt16 field and were clamped to 65535.");

        if (report.IntClampedCount > 0)
            report.Warnings.Add($"{report.IntClampedCount} GDMO Int32 fields exceeded the LKDMO signed Int32 field and were clamped.");

        if (report.InvalidMapMonsterReferenceCount > 0)
            report.Warnings.Add($"{report.InvalidMapMonsterReferenceCount} MapMonster rows referenced missing Monster.bin IDs and were skipped.");

        if (applyToPack)
        {
            var backups = Pack03Archive.PatchFiles(options.LkdmoInputPath, new Dictionary<string, string>
            {
                [MonsterRelativePath] = monsterPath,
                [MapMonsterRelativePath] = mapMonsterPath
            });
            report.PackBackupPf = backups.pfBackup;
            report.PackBackupHf = backups.hfBackup;
        }

        report.Success = report.Errors.Count == 0;
        return report;
    }

    private static MonsterSources ResolveSources(string gdmoPath)
    {
        var root = gdmoPath.Trim().Trim('"');
        var candidates = new List<string>();

        if (File.Exists(root))
            candidates.Add(Path.GetDirectoryName(root)!);
        else
            candidates.Add(root);

        candidates.Add(Path.Combine(root, "Data"));
        candidates.Add(Path.Combine(root, "Table"));
        candidates.Add(Path.Combine(root, "..", "xml", "AllBins_full2", "Data"));
        candidates.Add(Path.Combine(root, "..", "xml", "AllBins_full", "Data"));
        candidates.Add(Path.Combine(root, "..", "xml", "AllBins", "Data"));
        candidates.Add(Path.Combine(root, "..", "..", "xml", "AllBins_full2", "Data"));
        candidates.Add(Path.Combine(root, "..", "..", "xml", "AllBins_full", "Data"));
        candidates.Add(Path.Combine(root, "..", "..", "xml", "AllBins", "Data"));

        foreach (var candidate in candidates.Select(Path.GetFullPath).Distinct(StringComparer.OrdinalIgnoreCase))
        {
            var monster = Path.Combine(candidate, "Monster.xml");
            var skill = Path.Combine(candidate, "MonsterSkill.xml");
            var terms = Path.Combine(candidate, "MonsterSkillTerms.xml");
            var mapMonster = Path.Combine(candidate, "MapMonster.xml");
            var names = Path.GetFullPath(Path.Combine(candidate, "..", "_str", "MonsterName_Str.xml"));

            if (File.Exists(monster) && File.Exists(skill) && File.Exists(terms) && File.Exists(mapMonster) && File.Exists(names))
                return new MonsterSources(monster, skill, terms, mapMonster, names);
        }

        throw new FileNotFoundException($"Could not find GDMO monster XML export set from: {gdmoPath}.");
    }

    private static byte[] LoadCurrentMonster(string input)
    {
        var path = input.Trim().Trim('"');
        if (File.Exists(path) && Path.GetFileName(path).Equals("Monster.bin", StringComparison.OrdinalIgnoreCase))
            return File.ReadAllBytes(path);

        return Pack03Archive.ReadFile(path, MonsterRelativePath);
    }

    private static LegacyMonsterTemplate ParseLegacyMonster(byte[] bytes)
    {
        using var reader = new BinaryReader(new MemoryStream(bytes, writable: false), Encoding.Unicode);
        var monsterCount = reader.ReadInt32();
        var names = new Dictionary<uint, MonsterName>();

        for (var i = 0; i < monsterCount; i++)
        {
            var start = reader.BaseStream.Position;
            var monsterId = reader.ReadUInt32();
            _ = reader.ReadUInt32();
            var name = ReadFixedUtf16(reader, 64);
            var penName = ReadFixedUtf16(reader, 34);
            var title = ReadFixedUtf16(reader, 64);
            names.TryAdd(monsterId, new MonsterName(name, penName, title));
            reader.BaseStream.Position = start + MonsterRecordSize;
        }

        var hitCount = reader.ReadInt32();
        var hits = new List<HitRow>(hitCount);
        for (var i = 0; i < hitCount; i++)
            hits.Add(new HitRow(reader.ReadInt32(), reader.ReadInt32()));

        return new LegacyMonsterTemplate(names, hits);
    }

    private static Dictionary<uint, MonsterName> ParseMonsterNames(string path)
    {
        var doc = XDocument.Load(path);
        var result = new Dictionary<uint, MonsterName>();

        foreach (var element in doc.Descendants())
        {
            if (DirectChild(element, "TextKey") is null)
                continue;

            var id = RequiredUInt(element, "TextKey");
            var name = DirectChild(element, "TextA")?.Value ?? string.Empty;
            var penName = DirectChild(element, "TextB")?.Value ?? string.Empty;
            result.TryAdd(id, new MonsterName(name, penName, string.Empty));
        }

        return result;
    }

    private static Dictionary<uint, GdmoMonster> ParseGdmoMonsters(string path)
    {
        var doc = XDocument.Load(path);
        var result = new Dictionary<uint, GdmoMonster>();

        foreach (var element in doc.Descendants())
        {
            if (DirectChild(element, "MonsterId") is null || DirectChild(element, "MonsterTableId") is null)
                continue;

            var record = new GdmoMonster(
                RequiredUInt(element, "MonsterId"),
                RequiredUInt(element, "MonsterTableId"),
                RequiredUInt(element, "ModelId"),
                RequiredUShort(element, "Level"),
                RequiredUInt(element, "Exp"),
                RequiredUShort(element, "Battle"),
                RequiredUInt(element, "HP"),
                RequiredUInt(element, "DS"),
                RequiredUShort(element, "DE"),
                RequiredUShort(element, "EV"),
                RequiredUShort(element, "MS"),
                RequiredUShort(element, "CR"),
                RequiredUShort(element, "AT"),
                RequiredUShort(element, "AS"),
                RequiredUShort(element, "AR"),
                RequiredUShort(element, "HT"),
                RequiredUShort(element, "Sight"),
                RequiredUShort(element, "HuntRange"),
                RequiredFloat(element, "Scale"),
                RequiredUShort(element, "Class"),
                RequiredUShort(element, "Icon1"),
                RequiredUShort(element, "Icon2"),
                RequiredUShort(element, "Icon3"),
                RequiredUShort(element, "Icon4"),
                RequiredUShort(element, "Icon5"),
                RequiredUShort(element, "Icon6"),
                RequiredUShort(element, "NatureExpMin"),
                RequiredUShort(element, "NatureExpMax"));

            result.TryAdd(record.MonsterTableId, record);
        }

        return result;
    }

    private static List<GdmoMonsterSkill> ParseGdmoSkills(string path, MonsterConversionReport report)
    {
        var doc = XDocument.Load(path);
        var result = new List<GdmoMonsterSkill>();

        foreach (var element in doc.Descendants())
        {
            if (DirectChild(element, "SkillId") is null || DirectChild(element, "MonsterId") is null)
                continue;

            var factors = new List<SkillFactor>();
            var factorArray = DirectChild(element, "EffectFactors");
            if (factorArray is not null)
            {
                foreach (var factorElement in factorArray.Elements())
                {
                    if (DirectChild(factorElement, "EffectFactor") is null)
                        continue;
                    factors.Add(new SkillFactor(
                        RequiredUShort(factorElement, "EffectFactor"),
                        RequiredUInt(factorElement, "EffectFactorValue")));
                }
            }

            if (factors.Count > 3)
                report.SkillFactorTruncatedCount++;

            result.Add(new GdmoMonsterSkill(
                RequiredUShort(element, "SkillId"),
                RequiredUInt(element, "MonsterId"),
                RequiredUInt(element, "CoolTime"),
                RequiredUShort(element, "CastTime"),
                RequiredUShort(element, "CastCheck"),
                RequiredUShort(element, "TargetCount"),
                RequiredUShort(element, "TargetMinCount"),
                RequiredUShort(element, "TargetMaxCount"),
                RequiredUShort(element, "UseTerm"),
                RequiredUShort(element, "SkillType"),
                RequiredUInt(element, "EffectValueMin"),
                RequiredUInt(element, "EffectValueMax"),
                RequiredUShort(element, "RangeIndex"),
                RequiredUInt(element, "SequenceId"),
                RequiredUShort(element, "AniDelay"),
                RequiredUShort(element, "Velocity"),
                RequiredUShort(element, "Accel"),
                RequiredUInt(element, "TalkId"),
                RequiredUInt(element, "ActiveType"),
                RequiredFloat(element, "NoticeTime"),
                DirectChild(element, "NoticeEffectName")?.Value ?? string.Empty,
                factors));
        }

        return result.OrderBy(x => x.SkillId).ToList();
    }

    private static List<GdmoMonsterSkillTerms> ParseGdmoSkillTerms(string path)
    {
        var doc = XDocument.Load(path);
        var result = new List<GdmoMonsterSkillTerms>();

        foreach (var element in doc.Descendants())
        {
            if (DirectChild(element, "Index") is null)
                continue;

            result.Add(new GdmoMonsterSkillTerms(
                RequiredUShort(element, "Index"),
                RequiredUShort(element, "Direction"),
                RequiredUInt(element, "Range"),
                RequiredUShort(element, "TargetingType"),
                RequiredUShort(element, "RefCode")));
        }

        return result.OrderBy(x => x.Index).ToList();
    }

    private static List<GdmoMapMonsterSpawn> ParseGdmoMapMonster(string path)
    {
        var doc = XDocument.Load(path);
        var result = new List<GdmoMapMonsterSpawn>();

        foreach (var mapElement in doc.Descendants())
        {
            if (DirectChild(mapElement, "MapId") is null)
                continue;

            var mapId = RequiredUInt(mapElement, "MapId");
            var groups = DirectChild(mapElement, "Groups");
            if (groups is null)
                continue;

            foreach (var group in groups.Elements())
            {
                if (DirectChild(group, "MonsterTableId") is null)
                    continue;

                var monsterTableId = RequiredUInt(group, "MonsterTableId");
                var spawns = DirectChild(group, "Spawns");
                if (spawns is null)
                    continue;

                foreach (var spawn in spawns.Elements())
                {
                    if (DirectChild(spawn, "CenterX") is null)
                        continue;

                    result.Add(new GdmoMapMonsterSpawn(
                        mapId,
                        monsterTableId,
                        RequiredInt(spawn, "CenterX"),
                        RequiredInt(spawn, "CenterY"),
                        RequiredInt(spawn, "Radius"),
                        RequiredInt(spawn, "MoveType")));
                }
            }
        }

        return result;
    }

    private static byte[] BuildMonsterBin(
        IReadOnlyDictionary<uint, GdmoMonster> monsters,
        LegacyMonsterTemplate oldMonster,
        IReadOnlyDictionary<uint, MonsterName> gdmoNames,
        IReadOnlyList<GdmoMonsterSkill> skills,
        IReadOnlyList<GdmoMonsterSkillTerms> terms,
        MonsterConversionReport report)
    {
        using var output = new MemoryStream();
        using var writer = new BinaryWriter(output, Encoding.Unicode, leaveOpen: true);

        writer.Write(monsters.Count);
        foreach (var monster in monsters.Values.OrderBy(x => x.MonsterTableId))
            WriteMonsterRecord(writer, monster, oldMonster, gdmoNames, report);

        writer.Write(oldMonster.HitRows.Count);
        foreach (var hit in oldMonster.HitRows.OrderBy(x => x.Level))
        {
            writer.Write(hit.Level);
            writer.Write(hit.Hit);
        }

        writer.Write(skills.Count);
        foreach (var skill in skills)
            WriteSkillRecord(writer, skill, report);

        writer.Write(terms.Count);
        foreach (var term in terms)
            WriteSkillTermsRecord(writer, term);

        writer.Flush();
        report.OutputMonsterCount = monsters.Count;
        report.OutputSkillCount = skills.Count;
        report.OutputTermsCount = terms.Count;

        return output.ToArray();
    }

    private static void WriteMonsterRecord(
        BinaryWriter writer,
        GdmoMonster monster,
        LegacyMonsterTemplate oldMonster,
        IReadOnlyDictionary<uint, MonsterName> gdmoNames,
        MonsterConversionReport report)
    {
        var names = gdmoNames.TryGetValue(monster.MonsterTableId, out var gdmoName)
            ? gdmoName
            : oldMonster.NamesByMonsterId.TryGetValue(monster.MonsterTableId, out var oldName)
                ? oldName
                : new MonsterName(monster.MonsterTableId.ToString(), string.Empty, string.Empty);

        if (!gdmoNames.ContainsKey(monster.MonsterTableId))
            report.NameFallbackCount++;

        writer.Write(monster.MonsterTableId);
        writer.Write(monster.ModelId);
        WriteFixedUtf16(writer, names.Name, 64, report);
        WriteFixedUtf16(writer, names.PenName, 34, report);
        WriteFixedUtf16(writer, names.Title, 64, report);
        writer.Write(monster.Level);
        writer.Write(ClampUShort(monster.Exp, report));
        writer.Write(monster.Battle);
        writer.Write((ushort)0);
        writer.Write(ClampInt(monster.HP, report));
        writer.Write(ClampInt(monster.DS, report));
        writer.Write(monster.DE);
        writer.Write(monster.EV);
        writer.Write(monster.MS);
        writer.Write(monster.MS);
        writer.Write(monster.CR);
        writer.Write(monster.AT);
        writer.Write(monster.AS);
        writer.Write(monster.AR);
        writer.Write(monster.HT);
        writer.Write(monster.Sight);
        writer.Write(monster.HuntRange);
        writer.Write((ushort)0);
        writer.Write(monster.Scale);
        writer.Write(monster.Class);
        writer.Write(monster.Icon1);
        writer.Write(monster.Icon2);
        writer.Write(monster.Icon3);
        writer.Write(monster.Icon4);
        writer.Write(monster.Icon5);
        writer.Write(monster.Icon6);
        writer.Write(monster.NatureExpMin);
        writer.Write(monster.NatureExpMax);
        writer.Write((ushort)0);
    }

    private static void WriteSkillRecord(BinaryWriter writer, GdmoMonsterSkill skill, MonsterConversionReport report)
    {
        writer.Write(skill.SkillId);
        writer.Write((ushort)0);
        writer.Write(skill.MonsterId);
        writer.Write(skill.CoolTime);
        writer.Write(skill.CastTime);
        writer.Write(skill.CastCheck);
        writer.Write(skill.TargetCount);
        writer.Write(skill.TargetMinCount);
        writer.Write(skill.TargetMaxCount);
        writer.Write(skill.UseTerm);
        writer.Write(skill.SkillType);
        writer.Write((ushort)0);
        writer.Write(skill.EffectValueMin);
        writer.Write(skill.EffectValueMax);
        writer.Write(skill.RangeIndex);
        writer.Write((ushort)0);
        writer.Write(skill.SequenceId);
        writer.Write(skill.AniDelay);
        writer.Write(skill.Velocity);
        writer.Write(skill.Accel);

        var factors = skill.Factors.Take(3).ToArray();
        for (var i = 0; i < 3; i++)
            writer.Write(i < factors.Length ? factors[i].Factor : (ushort)0);
        for (var i = 0; i < 3; i++)
            writer.Write(i < factors.Length ? factors[i].Value : 0u);

        writer.Write(skill.TalkId);
        writer.Write(skill.ActiveType);
        writer.Write(skill.NoticeTime);
        WriteFixedBytes(writer, skill.NoticeEffectName, 64, report);
    }

    private static void WriteSkillTermsRecord(BinaryWriter writer, GdmoMonsterSkillTerms term)
    {
        writer.Write(term.Index);
        writer.Write(term.Direction);
        writer.Write(term.Range);
        writer.Write(term.TargetingType);
        writer.Write(term.RefCode);
    }

    private static byte[] BuildMapMonsterListBin(
        IReadOnlyList<GdmoMapMonsterSpawn> spawns,
        IReadOnlySet<uint> validMonsterIds,
        MonsterConversionReport report)
    {
        var validSpawns = spawns
            .Where(spawn =>
            {
                var valid = validMonsterIds.Contains(spawn.MonsterTableId);
                if (!valid) report.InvalidMapMonsterReferenceCount++;
                return valid;
            })
            .ToList();

        var byMonster = validSpawns
            .GroupBy(x => x.MonsterTableId)
            .OrderBy(x => x.Key)
            .ToList();

        using var output = new MemoryStream();
        using var writer = new BinaryWriter(output, Encoding.Unicode, leaveOpen: true);

        writer.Write(byMonster.Count);
        foreach (var monsterGroup in byMonster)
        {
            writer.Write(monsterGroup.Key);
            var byMap = monsterGroup.GroupBy(x => x.MapId).OrderBy(x => x.Key).ToList();
            writer.Write(byMap.Count);

            foreach (var mapGroup in byMap)
            {
                writer.Write(mapGroup.Key);
                writer.Write(mapGroup.Count());

                foreach (var spawn in mapGroup)
                {
                    writer.Write(spawn.MapId);
                    writer.Write(spawn.MonsterTableId);
                    writer.Write(spawn.CenterX);
                    writer.Write(spawn.CenterY);
                    writer.Write(spawn.Radius);
                    writer.Write(1);
                    writer.Write(DefaultRespawnSeconds);
                    writer.Write(0);
                    writer.Write(0);
                    writer.Write(0);
                    writer.Write(spawn.MoveType);
                    writer.Write(false);
                    writer.Write(new byte[3]);
                }
            }
        }

        writer.Flush();
        report.OutputMapMonsterFileTableCount = byMonster.Count;
        report.OutputMapSpawnCount = validSpawns.Count;
        return output.ToArray();
    }

    private static XElement? DirectChild(XElement element, string localName)
    {
        return element.Elements().FirstOrDefault(e => e.Name.LocalName == localName);
    }

    private static int RequiredInt(XElement element, string localName)
    {
        var text = DirectChild(element, localName)?.Attribute("value")?.Value;
        if (int.TryParse(text, out var value))
            return value;

        throw new InvalidDataException($"Missing or invalid {localName} in monster XML record.");
    }

    private static uint RequiredUInt(XElement element, string localName)
    {
        var text = DirectChild(element, localName)?.Attribute("value")?.Value;
        if (uint.TryParse(text, out var value))
            return value;

        throw new InvalidDataException($"Missing or invalid {localName} in monster XML record.");
    }

    private static ushort RequiredUShort(XElement element, string localName)
    {
        var value = RequiredUInt(element, localName);
        if (value > ushort.MaxValue)
            throw new InvalidDataException($"{localName} value {value} is outside UInt16 range.");

        return (ushort)value;
    }

    private static float RequiredFloat(XElement element, string localName)
    {
        var text = DirectChild(element, localName)?.Attribute("value")?.Value;
        if (float.TryParse(text, System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture, out var value))
            return value;

        throw new InvalidDataException($"Missing or invalid {localName} in monster XML record.");
    }

    private static ushort ClampUShort(uint value, MonsterConversionReport report)
    {
        if (value <= ushort.MaxValue)
            return (ushort)value;

        report.ExpClampedCount++;
        return ushort.MaxValue;
    }

    private static int ClampInt(uint value, MonsterConversionReport report)
    {
        if (value <= int.MaxValue)
            return (int)value;

        report.IntClampedCount++;
        return int.MaxValue;
    }

    private static string ReadFixedUtf16(BinaryReader reader, int chars)
    {
        var value = Encoding.Unicode.GetString(reader.ReadBytes(chars * 2));
        var nullIndex = value.IndexOf('\0');
        return nullIndex >= 0 ? value[..nullIndex] : value;
    }

    private static void WriteFixedUtf16(BinaryWriter writer, string value, int chars, MonsterConversionReport report)
    {
        value ??= string.Empty;
        if (value.Length >= chars)
        {
            value = value[..(chars - 1)];
            report.NameTruncatedCount++;
        }

        var buffer = new byte[chars * 2];
        Encoding.Unicode.GetBytes(value, 0, value.Length, buffer, 0);
        writer.Write(buffer);
    }

    private static void WriteFixedBytes(BinaryWriter writer, string value, int bytes, MonsterConversionReport report)
    {
        value ??= string.Empty;
        var raw = Encoding.UTF8.GetBytes(value);
        if (raw.Length > bytes)
        {
            Array.Resize(ref raw, bytes);
            report.NameTruncatedCount++;
        }

        var buffer = new byte[bytes];
        Array.Copy(raw, buffer, raw.Length);
        writer.Write(buffer);
    }

    private readonly record struct MonsterSources(
        string MonsterXml,
        string MonsterSkillXml,
        string MonsterSkillTermsXml,
        string MapMonsterXml,
        string MonsterNameXml);

    private readonly record struct LegacyMonsterTemplate(
        IReadOnlyDictionary<uint, MonsterName> NamesByMonsterId,
        IReadOnlyList<HitRow> HitRows);

    private readonly record struct HitRow(int Level, int Hit);

    private readonly record struct MonsterName(string Name, string PenName, string Title);

    private readonly record struct GdmoMonster(
        uint MonsterId,
        uint MonsterTableId,
        uint ModelId,
        ushort Level,
        uint Exp,
        ushort Battle,
        uint HP,
        uint DS,
        ushort DE,
        ushort EV,
        ushort MS,
        ushort CR,
        ushort AT,
        ushort AS,
        ushort AR,
        ushort HT,
        ushort Sight,
        ushort HuntRange,
        float Scale,
        ushort Class,
        ushort Icon1,
        ushort Icon2,
        ushort Icon3,
        ushort Icon4,
        ushort Icon5,
        ushort Icon6,
        ushort NatureExpMin,
        ushort NatureExpMax);

    private readonly record struct GdmoMonsterSkill(
        ushort SkillId,
        uint MonsterId,
        uint CoolTime,
        ushort CastTime,
        ushort CastCheck,
        ushort TargetCount,
        ushort TargetMinCount,
        ushort TargetMaxCount,
        ushort UseTerm,
        ushort SkillType,
        uint EffectValueMin,
        uint EffectValueMax,
        ushort RangeIndex,
        uint SequenceId,
        ushort AniDelay,
        ushort Velocity,
        ushort Accel,
        uint TalkId,
        uint ActiveType,
        float NoticeTime,
        string NoticeEffectName,
        IReadOnlyList<SkillFactor> Factors);

    private readonly record struct SkillFactor(ushort Factor, uint Value);

    private readonly record struct GdmoMonsterSkillTerms(
        ushort Index,
        ushort Direction,
        uint Range,
        ushort TargetingType,
        ushort RefCode);

    private readonly record struct GdmoMapMonsterSpawn(
        uint MapId,
        uint MonsterTableId,
        int CenterX,
        int CenterY,
        int Radius,
        int MoveType);
}
