using System.IO;
using System.Text;
using LkDmoBinConverter.Models;

namespace LkDmoBinConverter.Services;

public sealed class ItemListCraftConverter
{
    private const string ItemListRelativePath = @"data\bin\english\ItemList.bin";

    public Task<CraftConversionReport> ConvertCraftAsync(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        return Task.Run(() => ConvertCraft(options, applyToPack, cancellationToken), cancellationToken);
    }

    private static CraftConversionReport ConvertCraft(CraftConversionOptions options, bool applyToPack, CancellationToken cancellationToken)
    {
        cancellationToken.ThrowIfCancellationRequested();
        Directory.CreateDirectory(options.OutputDirectory);

        var report = new CraftConversionReport
        {
            OutputDirectory = options.OutputDirectory,
            Success = false
        };

        var sources = ResolveSources(options.GdmoBinPath);
        foreach (var source in sources.Values)
            report.SourceFiles.Add(source);

        var itemList = LoadCurrentItemList(options.LkdmoInputPath);
        var prefixOffset = GetItemListPrefixEnd(itemList);
        var oldCraft = ParseOldCraft(itemList, prefixOffset, requireEnd: false);
        var oldAssist = ParseAssist(itemList, oldCraft.EndOffset, requireEnd: false);
        var oldTailStart = oldAssist.EndOffset;

        var strings = ParseCreationStrings(File.ReadAllBytes(sources["strings"]));
        var convertedCraft = ConvertNewCreationToOld(File.ReadAllBytes(sources["creation"]), strings);
        var assistBytes = File.ReadAllBytes(sources["group"]);
        var newAssist = ParseAssist(assistBytes, 0, requireEnd: true);
        var limit = ParseLimitGroup(File.ReadAllBytes(sources["limit"]));

        var newItemList = new byte[prefixOffset + convertedCraft.Bytes.Length + assistBytes.Length + (itemList.Length - oldTailStart)];
        Buffer.BlockCopy(itemList, 0, newItemList, 0, prefixOffset);
        Buffer.BlockCopy(convertedCraft.Bytes, 0, newItemList, prefixOffset, convertedCraft.Bytes.Length);
        Buffer.BlockCopy(assistBytes, 0, newItemList, prefixOffset + convertedCraft.Bytes.Length, assistBytes.Length);
        Buffer.BlockCopy(itemList, oldTailStart, newItemList, prefixOffset + convertedCraft.Bytes.Length + assistBytes.Length, itemList.Length - oldTailStart);
        var belongingFix = NormalizeLegacyBelonging(newItemList);

        var itemListOutput = Path.Combine(options.OutputDirectory, "ItemList.bin");
        File.WriteAllBytes(itemListOutput, newItemList);
        File.Copy(sources["creation"], Path.Combine(options.OutputDirectory, "ItemCreation.bin"), overwrite: true);
        File.Copy(sources["group"], Path.Combine(options.OutputDirectory, "ItemCreationGroup.bin"), overwrite: true);
        File.Copy(sources["limit"], Path.Combine(options.OutputDirectory, "ItemCreationLimitGroup.bin"), overwrite: true);
        File.Copy(sources["strings"], Path.Combine(options.OutputDirectory, "ItemCreation_Str.bin"), overwrite: true);

        var validationPrefix = GetItemListPrefixEnd(newItemList);
        var validationCraft = ParseOldCraft(newItemList, validationPrefix, requireEnd: false);
        var validationAssist = ParseAssist(newItemList, validationCraft.EndOffset, requireEnd: false);
        if (validationCraft.Products != convertedCraft.Products)
            report.Errors.Add($"Validation craft product mismatch: expected {convertedCraft.Products}, got {validationCraft.Products}.");
        if (validationAssist.Groups != newAssist.Groups)
            report.Errors.Add($"Validation assist group mismatch: expected {newAssist.Groups}, got {validationAssist.Groups}.");

        report.PrefixOffset = prefixOffset;
        report.OldItemListSize = itemList.Length;
        report.NewItemListSize = newItemList.Length;
        report.OldCraftNpcs = oldCraft.NpcCount;
        report.NewCraftNpcs = convertedCraft.NpcCount;
        report.OldCraftProducts = oldCraft.Products;
        report.NewCraftProducts = convertedCraft.Products;
        report.NewMainCategories = convertedCraft.MainCategories;
        report.NewSubCategories = convertedCraft.SubCategories;
        report.NewMaterials = convertedCraft.Materials;
        report.OldAssistGroups = oldAssist.Groups;
        report.NewAssistGroups = newAssist.Groups;
        report.NewAssistItems = newAssist.Items;
        report.LimitGroupRecords = limit.Records;
        report.LimitGroupRecordSize = limit.RecordSize;
        report.ProductsWithLimitGroup = convertedCraft.LimitReferences;
        report.MissingCategoryNames = convertedCraft.MissingNames;
        report.LegacyBelongingFixes = belongingFix.Changed;
        foreach (var pair in belongingFix.Values)
            report.LegacyBelongingFixValues[pair.Key] = pair.Value;
        report.ItemListOutputPath = itemListOutput;
        report.EmbeddedItemListPatchEnabled = options.PatchEmbeddedItemListCraft;
        report.PatchedFiles.Add(itemListOutput);
        report.PatchedFiles.Add(Path.Combine(options.OutputDirectory, "ItemCreation.bin"));
        report.PatchedFiles.Add(Path.Combine(options.OutputDirectory, "ItemCreationGroup.bin"));
        report.PatchedFiles.Add(Path.Combine(options.OutputDirectory, "ItemCreationLimitGroup.bin"));
        report.PatchedFiles.Add(Path.Combine(options.OutputDirectory, "ItemCreation_Str.bin"));

        if (convertedCraft.LimitReferences > 0)
        {
            report.Warnings.Add("The legacy client does not read ItemCreationLimitGroup inside ItemList. The file is staged/patched as a sidecar, but enforcing those limits requires client/server support.");
        }

        if (convertedCraft.MissingNames > 0)
            report.Warnings.Add($"{convertedCraft.MissingNames} category names were missing and got fallback labels.");

        if (belongingFix.Changed > 0)
            report.Warnings.Add($"{belongingFix.Changed} ItemList records used belonging/bound values outside the legacy client range 0/1/2 and were normalized to 0.");

        if (!options.PatchEmbeddedItemListCraft)
        {
            report.Warnings.Add("Safe mode is enabled: ItemList.bin is generated for diff/analyze only and will not be patched into Pack03. The legacy client reads the craft window from embedded ItemList data, so enabling new craft UI still requires a client-safe embedded template.");
        }

        if (applyToPack)
        {
            var files = new Dictionary<string, string>
            {
                [@"data\bin\table\ItemCreation.bin"] = Path.Combine(options.OutputDirectory, "ItemCreation.bin"),
                [@"data\bin\table\ItemCreationGroup.bin"] = Path.Combine(options.OutputDirectory, "ItemCreationGroup.bin"),
                [@"data\bin\table\ItemCreationLimitGroup.bin"] = Path.Combine(options.OutputDirectory, "ItemCreationLimitGroup.bin"),
                [@"data\bin\english\ItemCreation_Str.bin"] = Path.Combine(options.OutputDirectory, "ItemCreation_Str.bin")
            };

            if (options.PatchEmbeddedItemListCraft)
                files[ItemListRelativePath] = itemListOutput;

            var backups = Pack03Archive.PatchFiles(options.LkdmoInputPath, files);
            report.PackBackupPf = backups.pfBackup;
            report.PackBackupHf = backups.hfBackup;
        }

        report.Success = report.Errors.Count == 0;
        return report;
    }

    private static LegacyBelongingFix NormalizeLegacyBelonging(byte[] itemList)
    {
        var itemCount = BitConverter.ToInt32(itemList, 0);
        if (itemCount < 0)
            throw new InvalidDataException($"ItemList has negative ItemInfo count {itemCount}.");

        var recordSize = BinaryLayout.ItemInfoSize();
        var belongingOffset = BinaryLayout.ItemInfoBelongingOffset();
        var itemInfoEnd = 4L + ((long)itemCount * recordSize);
        if (itemInfoEnd > itemList.Length)
            throw new InvalidDataException("ItemList ItemInfo section overflows the file.");

        var fix = new LegacyBelongingFix();
        for (var i = 0; i < itemCount; i++)
        {
            var offset = 4 + (i * recordSize) + belongingOffset;
            var value = itemList[offset];
            if (value is 0 or 1 or 2)
                continue;

            itemList[offset] = 0;
            fix.Changed++;
            fix.Values.TryGetValue(value, out var count);
            fix.Values[value] = count + 1;
        }

        return fix;
    }

    private static Dictionary<string, string> ResolveSources(string gdmoBinPath)
    {
        var root = gdmoBinPath.Trim().Trim('"');
        var candidates = new Dictionary<string, string[]>
        {
            ["creation"] =
            [
                Path.Combine(root, "Table", "ItemCreation.bin"),
                Path.Combine(root, "ItemCreation.bin"),
                Path.Combine(root, "data", "bin", "english", "ItemCreation.bin")
            ],
            ["group"] =
            [
                Path.Combine(root, "Table", "ItemCreationGroup.bin"),
                Path.Combine(root, "ItemCreationGroup.bin"),
                Path.Combine(root, "data", "bin", "english", "ItemCreationGroup.bin")
            ],
            ["limit"] =
            [
                Path.Combine(root, "Table", "ItemCreationLimitGroup.bin"),
                Path.Combine(root, "ItemCreationLimitGroup.bin"),
                Path.Combine(root, "data", "bin", "english", "ItemCreationLimitGroup.bin")
            ],
            ["strings"] =
            [
                Path.Combine(root, "Language", "English", "ItemCreation_Str.bin"),
                Path.Combine(root, "ItemCreation_Str.bin"),
                Path.Combine(root, "data", "bin", "english", "ItemCreation_Str.bin")
            ]
        };

        var result = new Dictionary<string, string>();
        foreach (var pair in candidates)
        {
            var file = pair.Value.FirstOrDefault(File.Exists);
            if (file is null)
                throw new FileNotFoundException($"Could not find {pair.Key} source under {root}.");
            result[pair.Key] = file;
        }

        return result;
    }

    private static byte[] LoadCurrentItemList(string input)
    {
        var path = input.Trim().Trim('"');
        if (File.Exists(path) && Path.GetFileName(path).Equals("ItemList.bin", StringComparison.OrdinalIgnoreCase))
            return File.ReadAllBytes(path);

        return Pack03Archive.ReadFile(path, ItemListRelativePath);
    }

    private static int GetItemListPrefixEnd(byte[] bytes)
    {
        using var reader = NewReader(bytes);
        var sections = new (string Name, int Size)[]
        {
            ("ItemInfo", BinaryLayout.ItemInfoSize()),
            ("ItemTap", 2 + (32 * 2)),
            ("CoolTime", BinaryLayout.Align(4 + 1, 8) + 8),
            ("MapDisp", 8),
            ("MapTypeName", 4 + (64 * 2)),
            ("Rank", 8),
            ("Element1", 4),
            ("Element2", 4),
            ("Exchange", BinaryLayout.ExchangeRecordSize()),
            ("AccessoryOption", BinaryLayout.AccessoryOptionRecordSize()),
            ("AccessoryEnchant", BinaryLayout.AccessoryEnchantRecordSize())
        };

        foreach (var section in sections)
        {
            var countOffset = reader.BaseStream.Position;
            var count = reader.ReadInt32();
            if (count < 0)
                throw new InvalidDataException($"ItemList section {section.Name} has negative count {count} at {countOffset}.");

            var skip = (long)count * section.Size;
            if (reader.BaseStream.Position + skip > bytes.Length)
                throw new InvalidDataException($"ItemList section {section.Name} overflows the file.");

            reader.BaseStream.Seek(skip, SeekOrigin.Current);
        }

        return checked((int)reader.BaseStream.Position);
    }

    private static Dictionary<uint, Dictionary<int, CreationStringMain>> ParseCreationStrings(byte[] bytes)
    {
        using var reader = NewReader(bytes);
        var npcCount = reader.ReadInt32();
        if (npcCount < 0 || npcCount > 20000)
            throw new InvalidDataException($"Invalid ItemCreation_Str NPC count {npcCount}.");

        var result = new Dictionary<uint, Dictionary<int, CreationStringMain>>();
        for (var n = 0; n < npcCount; n++)
        {
            var npc = reader.ReadUInt32();
            var mainCount = reader.ReadInt32();
            if (mainCount < 0 || mainCount > 5000)
                throw new InvalidDataException($"Invalid string main count {mainCount} for NPC {npc}.");

            var mains = new Dictionary<int, CreationStringMain>();
            for (var m = 0; m < mainCount; m++)
            {
                var mainKey = reader.ReadInt32();
                var main = new CreationStringMain { Name = ReadCountedUtf16(reader) };
                var subCount = reader.ReadInt32();
                if (subCount < 0 || subCount > 10000)
                    throw new InvalidDataException($"Invalid string sub count {subCount} for NPC {npc}, main {mainKey}.");

                for (var s = 0; s < subCount; s++)
                {
                    var subKey = reader.ReadInt32();
                    main.Subs[subKey] = ReadCountedUtf16(reader);
                }

                mains[mainKey] = main;
            }

            result[npc] = mains;
        }

        if (reader.BaseStream.Position != bytes.Length)
            throw new InvalidDataException($"ItemCreation_Str parsed to {reader.BaseStream.Position}, length is {bytes.Length}.");

        return result;
    }

    private static ConvertedCraftPayload ConvertNewCreationToOld(byte[] bytes, Dictionary<uint, Dictionary<int, CreationStringMain>> names)
    {
        using var reader = NewReader(bytes);
        using var output = new MemoryStream();
        using var writer = new BinaryWriter(output, Encoding.Unicode, leaveOpen: true);
        var npcCount = reader.ReadInt32();
        if (npcCount < 0 || npcCount > 20000)
            throw new InvalidDataException($"Invalid ItemCreation NPC count {npcCount}.");

        writer.Write(npcCount);
        var mainTotal = 0;
        var subTotal = 0;
        var products = 0;
        var materials = 0;
        var limits = 0;
        var missingNames = 0;

        for (var n = 0; n < npcCount; n++)
        {
            var npcKey = reader.ReadUInt32();
            var npcIdx = reader.ReadUInt32();
            var mainCount = reader.ReadInt32();
            if (mainCount < 0 || mainCount > 1000)
                throw new InvalidDataException($"Invalid main count {mainCount} for NPC {npcKey}/{npcIdx}.");

            mainTotal += mainCount;
            writer.Write(npcKey);
            writer.Write(mainCount);
            names.TryGetValue(npcKey, out var mainNames);
            mainNames ??= names.GetValueOrDefault(npcIdx);

            for (var m = 0; m < mainCount; m++)
            {
                var mainKey = reader.ReadInt32();
                var mainType = reader.ReadInt32();
                var subCount = reader.ReadInt32();
                if (subCount < 0 || subCount > 5000)
                    throw new InvalidDataException($"Invalid sub count {subCount} for NPC {npcKey}, main {mainKey}.");

                subTotal += subCount;
                var mainName = $"Category {mainKey}";
                Dictionary<int, string>? subNames = null;
                if (mainNames is not null && mainNames.TryGetValue(mainKey, out var mainString))
                {
                    mainName = mainString.Name;
                    subNames = mainString.Subs;
                }
                else
                {
                    missingNames++;
                }

                writer.Write(mainKey);
                writer.Write(mainType);
                WriteCountedUtf16(writer, mainName);
                writer.Write(subCount);

                for (var s = 0; s < subCount; s++)
                {
                    var subKey = reader.ReadInt32();
                    var subType = reader.ReadInt32();
                    var productCount = reader.ReadInt32();
                    if (productCount < 0 || productCount > 50000)
                        throw new InvalidDataException($"Invalid product count {productCount} for NPC {npcKey}, main {mainKey}, sub {subKey}.");

                    var subName = $"Subcategory {subKey}";
                    if (subNames is not null && subNames.TryGetValue(subKey, out var foundSubName))
                        subName = foundSubName;
                    else
                        missingNames++;

                    writer.Write(subKey);
                    writer.Write(subType);
                    WriteCountedUtf16(writer, subName);
                    writer.Write(productCount);
                    products += productCount;

                    for (var p = 0; p < productCount; p++)
                    {
                        var unique = reader.ReadInt32();
                        var itemIdx = reader.ReadUInt32();
                        var itemNum = reader.ReadInt32();
                        var probability = reader.ReadInt32();
                        var cost = reader.ReadUInt32();
                        var materialCount = reader.ReadInt32();
                        if (materialCount < 0 || materialCount > 200)
                            throw new InvalidDataException($"Invalid material count {materialCount} for unique {unique}, item {itemIdx}.");

                        var materialPairs = new (uint Item, int Num)[materialCount];
                        for (var mi = 0; mi < materialCount; mi++)
                            materialPairs[mi] = (reader.ReadUInt32(), reader.ReadInt32());

                        var percentAssistGroup = reader.ReadInt32();
                        var protectAssistGroup = reader.ReadInt32();
                        var limitGroup = reader.ReadInt32();
                        if (limitGroup != 0)
                            limits++;

                        writer.Write(unique);
                        writer.Write(itemIdx);
                        writer.Write(itemNum);
                        writer.Write(probability);
                        writer.Write(percentAssistGroup);
                        writer.Write(protectAssistGroup);
                        writer.Write(cost);
                        writer.Write(materialCount);
                        foreach (var pair in materialPairs)
                        {
                            writer.Write(pair.Item);
                            writer.Write(pair.Num);
                        }

                        materials += materialCount;
                    }
                }
            }
        }

        if (reader.BaseStream.Position != bytes.Length)
            throw new InvalidDataException($"ItemCreation parsed to {reader.BaseStream.Position}, length is {bytes.Length}.");

        writer.Flush();
        return new ConvertedCraftPayload
        {
            Bytes = output.ToArray(),
            NpcCount = npcCount,
            MainCategories = mainTotal,
            SubCategories = subTotal,
            Products = products,
            Materials = materials,
            LimitReferences = limits,
            MissingNames = missingNames
        };
    }

    private static CraftStats ParseOldCraft(byte[] bytes, int offset, bool requireEnd)
    {
        using var reader = NewReader(bytes);
        reader.BaseStream.Position = offset;
        var npcCount = reader.ReadInt32();
        if (npcCount < 0 || npcCount > 20000)
            throw new InvalidDataException($"Invalid legacy craft NPC count {npcCount} at {offset}.");

        var products = 0;
        var materials = 0;
        for (var n = 0; n < npcCount; n++)
        {
            _ = reader.ReadUInt32();
            var mainCount = reader.ReadInt32();
            if (mainCount < 0 || mainCount > 1000)
                throw new InvalidDataException($"Invalid legacy craft main count {mainCount}.");

            for (var m = 0; m < mainCount; m++)
            {
                _ = reader.ReadInt32();
                _ = reader.ReadInt32();
                _ = ReadCountedUtf16(reader);
                var subCount = reader.ReadInt32();
                if (subCount < 0 || subCount > 5000)
                    throw new InvalidDataException($"Invalid legacy craft sub count {subCount}.");

                for (var s = 0; s < subCount; s++)
                {
                    _ = reader.ReadInt32();
                    _ = reader.ReadInt32();
                    _ = ReadCountedUtf16(reader);
                    var productCount = reader.ReadInt32();
                    if (productCount < 0 || productCount > 50000)
                        throw new InvalidDataException($"Invalid legacy craft product count {productCount}.");

                    products += productCount;
                    for (var p = 0; p < productCount; p++)
                    {
                        _ = reader.ReadInt32();
                        _ = reader.ReadUInt32();
                        _ = reader.ReadInt32();
                        _ = reader.ReadInt32();
                        _ = reader.ReadInt32();
                        _ = reader.ReadInt32();
                        _ = reader.ReadUInt32();
                        var materialCount = reader.ReadInt32();
                        if (materialCount < 0 || materialCount > 200)
                            throw new InvalidDataException($"Invalid legacy craft material count {materialCount}.");

                        materials += materialCount;
                        for (var mi = 0; mi < materialCount; mi++)
                        {
                            _ = reader.ReadUInt32();
                            _ = reader.ReadInt32();
                        }
                    }
                }
            }
        }

        if (requireEnd && reader.BaseStream.Position != bytes.Length)
            throw new InvalidDataException($"Legacy craft parsed to {reader.BaseStream.Position}, length is {bytes.Length}.");

        return new CraftStats
        {
            NpcCount = npcCount,
            Products = products,
            Materials = materials,
            EndOffset = checked((int)reader.BaseStream.Position)
        };
    }

    private static AssistStats ParseAssist(byte[] bytes, int offset, bool requireEnd)
    {
        using var reader = NewReader(bytes);
        reader.BaseStream.Position = offset;
        var count = reader.ReadInt32();
        if (count < 0 || count > 100000)
            throw new InvalidDataException($"Invalid assist group count {count} at {offset}.");

        var items = 0;
        for (var g = 0; g < count; g++)
        {
            _ = reader.ReadInt32();
            _ = reader.ReadInt32();
            var itemSize = reader.ReadInt32();
            if (itemSize < 0 || itemSize > 5000)
                throw new InvalidDataException($"Invalid assist item size {itemSize}.");

            items += itemSize;
            for (var i = 0; i < itemSize; i++)
            {
                _ = reader.ReadUInt32();
                _ = reader.ReadInt32();
            }
        }

        if (requireEnd && reader.BaseStream.Position != bytes.Length)
            throw new InvalidDataException($"Assist group parsed to {reader.BaseStream.Position}, length is {bytes.Length}.");

        return new AssistStats
        {
            Groups = count,
            Items = items,
            EndOffset = checked((int)reader.BaseStream.Position)
        };
    }

    private static LimitGroupStats ParseLimitGroup(byte[] bytes)
    {
        using var reader = NewReader(bytes);
        var count = reader.ReadInt32();
        if (count < 0)
            throw new InvalidDataException($"Invalid limit group count {count}.");

        var remaining = bytes.Length - 4;
        var recordSize = count > 0 && remaining % count == 0 ? remaining / count : -1;
        return new LimitGroupStats { Records = count, RecordSize = recordSize };
    }

    private static BinaryReader NewReader(byte[] bytes) => new(new MemoryStream(bytes, writable: false), Encoding.Unicode);

    private static string ReadCountedUtf16(BinaryReader reader)
    {
        var len = reader.ReadInt32();
        if (len < 0 || len > 2048)
            throw new InvalidDataException($"Invalid UTF-16 length {len} at {reader.BaseStream.Position - 4}.");

        var bytes = reader.ReadBytes(len * 2);
        if (bytes.Length != len * 2)
            throw new EndOfStreamException("Unexpected EOF while reading UTF-16 string.");

        return Encoding.Unicode.GetString(bytes);
    }

    private static void WriteCountedUtf16(BinaryWriter writer, string value)
    {
        value ??= string.Empty;
        writer.Write(value.Length);
        if (value.Length > 0)
            writer.Write(Encoding.Unicode.GetBytes(value));
    }

    private sealed class LegacyBelongingFix
    {
        public int Changed { get; set; }
        public Dictionary<int, int> Values { get; } = [];
    }
}
