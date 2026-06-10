using DigitalWorldOnline.Commons.Models.Asset;

namespace DigitalWorldOnline.Application.GameAssets.Bins;

public sealed class ExtraExchangeBinLoader
{
    private const string FileName = "ExtraExchange.bin";
    private IReadOnlyList<ExtraEvolutionNpcAssetModel>? _data;

    public IReadOnlyList<ExtraEvolutionNpcAssetModel> Data => _data ?? throw new InvalidOperationException(
        $"{nameof(ExtraExchangeBinLoader)}: bin not loaded yet - call Load() first.");

    public bool IsLoaded => _data != null;

    public IReadOnlyList<ExtraEvolutionNpcAssetModel> Load()
    {
        if (_data != null) return _data;

        var path = Path.Combine(BinPath.ResolveDirectory(), FileName);
        if (!File.Exists(path))
        {
            _data = Array.Empty<ExtraEvolutionNpcAssetModel>();
            return _data;
        }

        using var fs = File.OpenRead(path);
        using var r = new BinaryReader(fs);

        _data = Parse(r);
        return _data;
    }

    private static IReadOnlyList<ExtraEvolutionNpcAssetModel> Parse(BinaryReader r)
    {
        var npcCount = checked((int)r.ReadUInt32());
        var npcs = new List<ExtraEvolutionNpcAssetModel>(npcCount);

        long npcAssetId = 1;
        long infoAssetId = 1;
        long extraAssetId = 1;
        long materialAssetId = 1;
        long requiredAssetId = 1;

        for (var npcIndex = 0; npcIndex < npcCount; npcIndex++)
        {
            var npc = new ExtraEvolutionNpcAssetModel
            {
                Id = npcAssetId++,
                NpcId = checked((int)r.ReadUInt32()),
                ExtraEvolutionInformation = new List<ExtraEvolutionInformationAssetModel>()
            };

            var subgroupCount = checked((int)r.ReadUInt32());
            for (var subgroupIndex = 0; subgroupIndex < subgroupCount; subgroupIndex++)
            {
                var info = new ExtraEvolutionInformationAssetModel
                {
                    Id = infoAssetId++,
                    IndexId = r.ReadUInt16(),
                    ExtraEvolution = new List<ExtraEvolutionAssetModel>()
                };

                var entryCount = checked((int)r.ReadUInt32());
                for (var entryIndex = 0; entryIndex < entryCount; entryIndex++)
                {
                    var objectId = checked((int)r.ReadUInt32());
                    _ = r.ReadUInt16();
                    var requiredLevel = checked((int)r.ReadUInt32());
                    var price = r.ReadUInt32();
                    var wayType = r.ReadUInt16();

                    var extra = new ExtraEvolutionAssetModel
                    {
                        Id = extraAssetId++,
                        DigimonId = objectId,
                        RequiredLevel = (byte)Math.Clamp(requiredLevel, 0, byte.MaxValue),
                        WayType = wayType,
                        Price = price,
                        Requireds = new List<ExtraEvolutionRequiredAssetModel>(),
                        Materials = new List<ExtraEvolutionMaterialAssetModel>()
                    };

                    var mainMaterialCount = checked((int)r.ReadUInt32());
                    for (var materialIndex = 0; materialIndex < mainMaterialCount; materialIndex++)
                    {
                        extra.Requireds.Add(new ExtraEvolutionRequiredAssetModel
                        {
                            Id = requiredAssetId++,
                            ItemId = checked((int)r.ReadUInt32()),
                            Amount = NormalizeAmount(r.ReadUInt32())
                        });
                    }

                    var subMaterialCount = checked((int)r.ReadUInt32());
                    for (var materialIndex = 0; materialIndex < subMaterialCount; materialIndex++)
                    {
                        extra.Materials.Add(new ExtraEvolutionMaterialAssetModel
                        {
                            Id = materialAssetId++,
                            ItemId = checked((int)r.ReadUInt32()),
                            Amount = NormalizeAmount(r.ReadUInt32())
                        });
                    }

                    info.ExtraEvolution.Add(extra);
                }

                npc.ExtraEvolutionInformation.Add(info);
            }

            npcs.Add(npc);
        }

        if (r.BaseStream.Position != r.BaseStream.Length)
            throw new InvalidDataException($"{FileName}: trailing bytes after parse.");

        return npcs;
    }

    private static int NormalizeAmount(uint value)
    {
        if (value == 0)
            return 1;

        return checked((int)value);
    }
}
