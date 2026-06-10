namespace DigitalWorldOnline.Commons.Models.Character
{
    public sealed partial class CharacterProgressModel
    {
        public const int QuestSlotsPerSet = 3072;
        public const int QuestSetCount = 3;
        public const int CompletedQuestIntCount = QuestSlotsPerSet / 32 * QuestSetCount;
        public const int CompletedQuestByteCount = CompletedQuestIntCount * 4;
        public const int MaxInProgressQuestCount = 20;
        public const int InProgressQuestPacketSize = 7;
        public const int SerializedQuestProgressSize = CompletedQuestByteCount + MaxInProgressQuestCount * InProgressQuestPacketSize;
        public const int MaxTrackedQuestId = QuestSlotsPerSet * QuestSetCount;
        /// <summary>
        /// Unique sequential identifier.
        /// </summary>
        public long Id { get; private set; }

        /// <summary>
        /// Byte array for completed quests (through bitwise operation).
        /// </summary>
        public byte[] CompletedData { get; set; } = new byte[CompletedQuestByteCount];
        
        /// <summary>
        /// Byte array for completed achievements (through bitwise operation).
        /// </summary>
        //public byte[] CompletedAchievmentData { get; private set; } = new byte[340];
        
        /// <summary>
        /// Client id references for achievements and quests.
        /// </summary>
        public int[] CompletedDataValue { get;  set; } = new int[CompletedQuestIntCount];

        /// <summary>
        /// In progress quest list.
        /// </summary>
        public List<InProgressQuestModel> InProgressQuestData { get;  set; } = new();

        public void EnsureQuestProgressCapacity()
        {
            CompletedData = EnsureCompletedDataCapacity(CompletedData);
            CompletedDataValue = EnsureCompletedDataValueCapacity(CompletedDataValue);
        }

        public static byte[] EnsureCompletedDataCapacity(byte[] source)
        {
            var result = new byte[CompletedQuestByteCount];

            if (source != null)
                System.Array.Copy(source, result, System.Math.Min(source.Length, result.Length));

            return result;
        }

        public static int[] EnsureCompletedDataValueCapacity(int[] source)
        {
            var result = new int[CompletedQuestIntCount];

            if (source != null)
                System.Array.Copy(source, result, System.Math.Min(source.Length, result.Length));

            return result;
        }
    }
}
