


namespace DigitalWorldOnline.Commons.Models.Config
{
    public sealed partial class KillSpawnSourceMobConfigModel
    {
        /// <summary>
        /// Decrease the current source mob kill amount.
        /// </summary>
        public void DecreaseCurrentSourceMobAmount(byte amount = 1)
        {
            if (CurrentSourceMobRequiredAmount == 0)
                return;

            if (amount >= CurrentSourceMobRequiredAmount)
            {
                CurrentSourceMobRequiredAmount = 0;
                return;
            }

            CurrentSourceMobRequiredAmount -= amount;
        }

        /// <summary>
        /// Resets the current source mob kill amount.
        /// </summary>
        public void ResetCurrentSourceMobAmount()
        {
            CurrentSourceMobRequiredAmount = SourceMobRequiredAmount;
        }
    }
}
