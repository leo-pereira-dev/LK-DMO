using DigitalWorldOnline.Commons.DTOs.Config;
using DigitalWorldOnline.Commons.Enums;
using DigitalWorldOnline.Commons.Enums.ClientEnums;

namespace DigitalWorldOnline.Infraestructure.Repositories.Config
{
    internal static class MobDropRewardBinLoader
    {
        private const uint Magic = 0x4244524D; // MDRB, little-endian
        private const int Version = 1;
        private const string FileName = "MobDropReward.bin";

        public static IReadOnlyList<MobConfigDTO>? LoadIfExists()
        {
            var path = Path.Combine(ResolveBinDirectory(), FileName);
            if (!File.Exists(path))
                return null;

            using var fs = File.OpenRead(path);
            using var reader = new BinaryReader(fs);

            var magic = reader.ReadUInt32();
            if (magic != Magic)
                throw new InvalidDataException($"{FileName}: invalid magic 0x{magic:X8}.");

            var version = reader.ReadInt32();
            if (version != Version)
                throw new InvalidDataException($"{FileName}: unsupported version {version}.");

            var count = reader.ReadInt32();
            if (count < 0)
                throw new InvalidDataException($"{FileName}: invalid mob count {count}.");

            var mobs = new List<MobConfigDTO>(count);
            for (var i = 0; i < count; i++)
                mobs.Add(ReadMob(reader));

            if (fs.Position != fs.Length)
                throw new InvalidDataException($"{FileName}: parsed to {fs.Position}, length is {fs.Length}.");

            return mobs;
        }

        private static MobConfigDTO ReadMob(BinaryReader reader)
        {
            var mob = new MobConfigDTO
            {
                Id = reader.ReadInt64(),
                Type = reader.ReadInt32(),
                Model = reader.ReadInt32(),
                Name = ReadString(reader),
                Level = reader.ReadByte(),
                ViewRange = reader.ReadInt32(),
                HuntRange = reader.ReadInt32(),
                Class = reader.ReadInt32(),
                Coliseum = reader.ReadBoolean(),
                Round = reader.ReadByte(),
                WeekDay = (DungeonDayOfWeekEnum)reader.ReadInt32(),
                ColiseumMobType = (ColiseumMobTypeEnum)reader.ReadInt32(),
                ReactionType = (DigimonReactionTypeEnum)reader.ReadInt32(),
                Attribute = (DigimonAttributeEnum)reader.ReadInt32(),
                Element = (DigimonElementEnum)reader.ReadInt32(),
                Family1 = (DigimonFamilyEnum)reader.ReadInt32(),
                Family2 = (DigimonFamilyEnum)reader.ReadInt32(),
                Family3 = (DigimonFamilyEnum)reader.ReadInt32(),
                RespawnInterval = reader.ReadInt32(),
                GameMapConfigId = reader.ReadInt64(),
                ASValue = reader.ReadInt32(),
                ARValue = reader.ReadInt32(),
                ATValue = reader.ReadInt32(),
                BLValue = reader.ReadInt32(),
                CTValue = reader.ReadInt32(),
                DEValue = reader.ReadInt32(),
                DSValue = reader.ReadInt32(),
                EVValue = reader.ReadInt32(),
                HPValue = reader.ReadInt32(),
                HTValue = reader.ReadInt32(),
                MSValue = reader.ReadInt32(),
                WSValue = reader.ReadInt32()
            };

            mob.GameMapConfig = new MapConfigDTO
            {
                Id = mob.GameMapConfigId,
                MapId = reader.ReadInt32(),
                Name = ReadString(reader),
                Type = (MapTypeEnum)reader.ReadInt32(),
                Mobs = new List<MobConfigDTO>(),
                KillSpawns = new List<KillSpawnConfigDTO>()
            };

            mob.Location = new MobLocationConfigDTO
            {
                Id = reader.ReadInt64(),
                MobConfigId = reader.ReadInt64(),
                MapId = reader.ReadInt16(),
                X = reader.ReadInt32(),
                Y = reader.ReadInt32(),
                Z = reader.ReadSingle()
            };

            mob.DropReward = new MobDropRewardConfigDTO
            {
                Id = reader.ReadInt64(),
                MobId = mob.Id,
                MinAmount = reader.ReadByte(),
                MaxAmount = reader.ReadByte()
            };

            var hasBits = reader.ReadBoolean();
            mob.DropReward.BitsDrop = hasBits
                ? new BitsDropConfigDTO
                {
                    Id = reader.ReadInt64(),
                    DropRewardId = reader.ReadInt64(),
                    MinAmount = reader.ReadInt32(),
                    MaxAmount = reader.ReadInt32(),
                    Chance = reader.ReadDouble()
                }
                : new BitsDropConfigDTO();

            var dropCount = reader.ReadInt32();
            if (dropCount < 0)
                throw new InvalidDataException($"{FileName}: invalid item drop count {dropCount}.");

            mob.DropReward.Drops = new List<ItemDropConfigDTO>(dropCount);
            for (var i = 0; i < dropCount; i++)
            {
                mob.DropReward.Drops.Add(new ItemDropConfigDTO
                {
                    Id = reader.ReadInt64(),
                    DropRewardId = reader.ReadInt64(),
                    ItemId = reader.ReadInt32(),
                    MinAmount = reader.ReadInt32(),
                    MaxAmount = reader.ReadInt32(),
                    Chance = reader.ReadDouble(),
                    Rank = reader.ReadInt32()
                });
            }

            return mob;
        }

        private static string ReadString(BinaryReader reader)
        {
            var length = reader.ReadInt32();
            if (length < 0 || length > 4096)
                throw new InvalidDataException($"{FileName}: invalid string length {length}.");

            var bytes = reader.ReadBytes(length);
            if (bytes.Length != length)
                throw new EndOfStreamException($"{FileName}: unexpected EOF while reading string.");

            return System.Text.Encoding.UTF8.GetString(bytes);
        }

        private static string ResolveBinDirectory()
        {
            var dir = new DirectoryInfo(AppContext.BaseDirectory);
            while (dir != null)
            {
                var candidate = Path.Combine(dir.FullName, "Bins", "data", "bin", "english");
                if (Directory.Exists(candidate))
                    return candidate;

                dir = dir.Parent;
            }

            throw new DirectoryNotFoundException(
                "Could not locate 'Bins/data/bin/english/' walking up from " +
                $"AppContext.BaseDirectory ({AppContext.BaseDirectory}).");
        }
    }
}
