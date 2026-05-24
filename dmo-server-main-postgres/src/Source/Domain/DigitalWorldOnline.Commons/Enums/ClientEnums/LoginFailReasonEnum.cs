namespace DigitalWorldOnline.Commons.Enums.ClientEnums
{
    public enum LoginFailReasonEnum
    {
        /// <summary>
        /// Invalid username or password. Maps to DM_Message 10002 in the English client.
        /// </summary>
        UserNotFound = 10002,

        /// <summary>
        /// Generic server/account error message. Account bans normally use LoginRequestBannedAnswerPacket.
        /// </summary>
        BannedAccount = 10034,

        /// <summary>
        /// Invalid username or password. Maps to DM_Message 10002 in the English client.
        /// </summary>
        IncorrectPassword = 10002
    }
}
