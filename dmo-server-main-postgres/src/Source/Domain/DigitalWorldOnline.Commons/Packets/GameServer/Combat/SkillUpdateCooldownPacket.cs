using DigitalWorldOnline.Commons.Models.Digimon;
using DigitalWorldOnline.Commons.Writers;

namespace DigitalWorldOnline.Commons.Packets.GameServer.Combat
{
    public class SkillUpdateCooldownPacket : PacketWriter
    {
        private const int PacketNumber = 3246;
   
        /// <summary>
        /// Set the target as out of combat.
        /// </summary>
        /// <param name="handler">The target handler to set</param>
        public SkillUpdateCooldownPacket(int handler, int currentType, DigimonEvolutionModel? evolution, List<int> skillIds)
        {
            Type(PacketNumber);
            WriteUShort((ushort)handler);
            WriteInt(currentType);

            var activeCooldowns = evolution?.Skills
                .Select((skill, index) => new { Skill = skill, Index = index })
                .Where(entry => entry.Skill.RemainingSeconds > 0 && entry.Index < skillIds.Count)
                .ToList() ?? new();

            WriteInt(activeCooldowns.Count);

            foreach (var entry in activeCooldowns)
            {
                WriteInt(skillIds[entry.Index]);
                var endTime = DateTimeOffset.UtcNow.AddSeconds(entry.Skill.RemainingSeconds).ToUnixTimeSeconds();
                WriteInt((int)endTime);
            }
        }
    }
}
