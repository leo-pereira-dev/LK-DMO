namespace DigitalWorldOnline.Commons.Models.Character
{
    public sealed partial class CharacterXmlUnionProgressModel
    {
        public long Id { get; private set; }
        public long CharacterId { get; private set; }
        public byte Level { get; private set; }
        public int CurrentExperience { get; private set; }
        public int RequiredExperience { get; private set; }
        public ushort ClaimedRewardMask { get; private set; }
        public DateTime CreatedAt { get; private set; }
        public DateTime UpdatedAt { get; private set; }

        public CharacterXmlUnionProgressModel()
        {
            RequiredExperience = 10000;
            CreatedAt = DateTime.UtcNow;
            UpdatedAt = DateTime.UtcNow;
        }
    }
}
