namespace DigitalWorldOnline.Commons.Models.Character
{
    public sealed partial class CharacterProgressModel
    {
        /// <summary>
        /// Serializes the progress object.
        /// </summary>
        public byte[] ToArray()
        {
            byte[] buffer = Array.Empty<byte>();

            EnsureQuestProgressCapacity();

            using (MemoryStream m = new(SerializedQuestProgressSize))
            {
                Buffer.BlockCopy(CompletedDataValue, 0, CompletedData, 0, CompletedData.Length);

                //var temp = new byte[340];

                //m.Write(temp.ToArray(), 0, 340);
                m.Write(CompletedData.ToArray(), 0, CompletedData.Length);

                foreach (var inProgressQuest in InProgressQuestData)
                    m.Write(inProgressQuest.ToArray(), 0, InProgressQuestPacketSize);

                var remaningQuestsToAdd = MaxInProgressQuestCount - InProgressQuestData.Count;
                for (int i = 0; i < remaningQuestsToAdd; i++)
                    m.Write(new InProgressQuestModel().ToArray(), 0, InProgressQuestPacketSize);

                buffer = m.ToArray();
            }

            return buffer;
        }

        /// <summary>
        /// Adds a new quest to the current quests list.
        /// </summary>
        /// <param name="questId">Quest identifier</param>
        /// <returns>True when the tamer has free quest slots to add this quest.</returns>
        public bool AcceptQuest(short questId)
        {
            if (InProgressQuestData.Any(x => x.QuestId == questId))
                return false;

            if (InProgressQuestData.Count >= MaxInProgressQuestCount)
                return false;

            InProgressQuestData.Add(new InProgressQuestModel(questId));

            return true;
        }

        public bool HasQuestInProgress(short questId)
        {
            return InProgressQuestData.Any(x => x.QuestId == questId);
        }

        public bool IsQuestCompleted(int questId)
        {
            if (questId <= 0 || questId > MaxTrackedQuestId)
                return false;

            EnsureQuestProgressCapacity();

            var bitIndex = questId - 1;
            var arrayIndex = bitIndex / 32;
            var bitPosition = bitIndex % 32;

            return ((CompletedDataValue[arrayIndex] >> bitPosition) & 1) == 1;
        }

        public bool MarkQuestCompleted(int questId)
        {
            if (questId <= 0 || questId > MaxTrackedQuestId)
                return false;

            EnsureQuestProgressCapacity();

            var bitIndex = questId - 1;
            var arrayIndex = bitIndex / 32;
            var bitPosition = bitIndex % 32;
            var mask = 1 << bitPosition;

            if ((CompletedDataValue[arrayIndex] & mask) != 0)
                return false;

            CompletedDataValue[arrayIndex] |= mask;
            return true;
        }

        /// <summary>
        /// Removes the quest from the current quests list.
        /// </summary>
        /// <param name="questId">Quest identifier</param>
        public Guid? RemoveQuest(short questId)
        {
            var progress = InProgressQuestData.FirstOrDefault(x => x.QuestId == questId);
            InProgressQuestData.RemoveAll(x => x.QuestId == questId);

            return progress?.Id;
        }

        /// <summary>
        /// Updates quest progress?
        /// </summary>
        public void UpdateQuestInProgress(short questId, int goalIndex, byte value)
        {
            InProgressQuestData.FirstOrDefault(x => x.QuestId == questId)?.UpdateCondition(goalIndex, value);
        }

        public byte GetQuestGoalProgress(short questId, int goalIndex)
        {
            return InProgressQuestData.First(x => x.QuestId == questId).GetGoalValue(goalIndex);
        }
    }
}
