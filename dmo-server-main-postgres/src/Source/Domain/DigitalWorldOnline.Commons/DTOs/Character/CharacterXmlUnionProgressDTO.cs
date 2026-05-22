namespace DigitalWorldOnline.Commons.DTOs.Character
{
    public sealed class CharacterXmlUnionProgressDTO
    {
        public long Id { get; set; }
        public byte Level { get; set; }
        public int CurrentExperience { get; set; }
        public int RequiredExperience { get; set; }
        public ushort ClaimedRewardMask { get; set; }
        public DateTime CreatedAt { get; set; }
        public DateTime UpdatedAt { get; set; }

        //FK
        public CharacterDTO Character { get; set; }
        public long CharacterId { get; set; }
    }
}
