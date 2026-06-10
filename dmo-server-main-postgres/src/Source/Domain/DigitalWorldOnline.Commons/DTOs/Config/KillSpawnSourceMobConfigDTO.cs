

namespace DigitalWorldOnline.Commons.DTOs.Config
{
    public sealed class KillSpawnSourceMobConfigDTO
    {
        public long Id { get; set; }

        /// <summary>
        /// Source mob identifier.
        /// </summary>
        public int SourceMobType { get; set; }

        /// <summary>
        /// Source mob kill amount.
        /// </summary>
        public byte SourceMobRequiredAmount { get; set; }

        /// <summary>
        /// Reference to the owner.
        /// </summary>
        public long KillSpawnId { get; set; }
        public KillSpawnConfigDTO KillSpawn { get; set; }
    }
}
