namespace DigitalWorldOnline.Api.Dtos.In
{
    public class LoginAccountIn
    {
        /// <summary>
        /// Username.
        /// </summary>
        public string Username { get; set; } = string.Empty;

        /// <summary>
        /// Password.
        /// </summary>
        public string Password { get; set; } = string.Empty;
    }
}
