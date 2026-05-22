using DigitalWorldOnline.Application.GameAssets.Xml;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer
{
    public sealed class XmlUnionCollectionInfoPacket : PacketWriter
    {
        public const int PacketNumber = 11053;

        public XmlUnionCollectionInfoPacket(DUnitCollectionBonusResult result)
        {
            Type(PacketNumber);

            WriteUShort((ushort)Math.Min(result.Groups.Count, ushort.MaxValue));
            foreach (var group in result.Groups.Take(ushort.MaxValue))
            {
                WriteUInt(ClientGroupId(group.Group));
                WriteByte(group.IsCompleted ? (byte)1 : (byte)0);
                WriteByte((byte)Math.Clamp(group.OwnedDigimonCount, 0, byte.MaxValue));
                WriteUShort((ushort)Math.Clamp(group.TotalLevel, 0, ushort.MaxValue));
                WriteByte(group.AllRequiredTranscended ? (byte)1 : (byte)0);

                WriteByte((byte)Math.Min(group.RequiredDigimons.Count, byte.MaxValue));
                foreach (var member in group.RequiredDigimons.Take(byte.MaxValue))
                {
                    WriteInt(member.Requirement.DigimonId);
                    WriteByte(member.Requirement.Slot);
                    WriteByte(member.Owned ? (byte)1 : (byte)0);
                    WriteUShort(member.OwnedLevel);
                    WriteByte(member.EvolutionUnlocked ? (byte)1 : (byte)0);
                    WriteByte(member.Transcended ? (byte)1 : (byte)0);
                    WriteByte(member.MeetsLevel ? (byte)1 : (byte)0);
                    WriteByte(member.MeetsEvolution ? (byte)1 : (byte)0);
                    WriteByte(member.MeetsTranscendence ? (byte)1 : (byte)0);
                }

                WriteByte((byte)Math.Min(group.Effects.Count, byte.MaxValue));
                foreach (var effect in group.Effects.Take(byte.MaxValue))
                {
                    WriteByte((byte)Math.Clamp(effect.Effect.RequiredConditionIndex ?? 0, 0, byte.MaxValue));
                    WriteByte(effect.Active ? (byte)1 : (byte)0);
                    WriteByte((byte)effect.Effect.EffectType);
                    WriteInt(effect.Effect.EffectValue);
                }
            }

            WriteInt(result.Get(DUnitCollectionEffectType.HP));
            WriteInt(result.Get(DUnitCollectionEffectType.DS));
            WriteInt(result.Get(DUnitCollectionEffectType.AT));
            WriteInt(result.Get(DUnitCollectionEffectType.DE));
            WriteInt(result.Get(DUnitCollectionEffectType.EV));
            WriteInt(result.Get(DUnitCollectionEffectType.HT));
            WriteInt(result.Get(DUnitCollectionEffectType.CT));
            WriteInt(result.Get(DUnitCollectionEffectType.EXP));
            WriteInt(result.Get(DUnitCollectionEffectType.BL));
            WriteInt(result.Get(DUnitCollectionEffectType.SCD));
            WriteInt(result.Get(DUnitCollectionEffectType.Basic));

            WriteInt(result.Get(DUnitCollectionEffectType.Vaccine));
            WriteInt(result.Get(DUnitCollectionEffectType.Data));
            WriteInt(result.Get(DUnitCollectionEffectType.Virus));
            WriteInt(result.Get(DUnitCollectionEffectType.UnknownAttribute));
            WriteInt(result.Get(DUnitCollectionEffectType.Fire));
            WriteInt(result.Get(DUnitCollectionEffectType.Water));
            WriteInt(result.Get(DUnitCollectionEffectType.Ice));
            WriteInt(result.Get(DUnitCollectionEffectType.Wind));
            WriteInt(result.Get(DUnitCollectionEffectType.Thunder));
            WriteInt(result.Get(DUnitCollectionEffectType.Light));
            WriteInt(result.Get(DUnitCollectionEffectType.Dark));
            WriteInt(result.Get(DUnitCollectionEffectType.Land));
            WriteInt(result.Get(DUnitCollectionEffectType.Wood));
            WriteInt(result.Get(DUnitCollectionEffectType.Steel));
        }

        private static uint ClientGroupId(DUnitCollectionGroup group)
        {
            return string.Equals(group.SourceType, "Collection", StringComparison.OrdinalIgnoreCase)
                ? (uint)(group.GroupId + 100000)
                : (uint)group.GroupId;
        }
    }
}
