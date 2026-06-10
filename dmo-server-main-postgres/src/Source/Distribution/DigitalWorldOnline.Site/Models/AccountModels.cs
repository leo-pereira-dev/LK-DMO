using System.ComponentModel.DataAnnotations;

namespace DigitalWorldOnline.Site.Models;

public sealed class LoginFormModel
{
    [Required(ErrorMessage = "Informe o usuario.")]
    [MinLength(6, ErrorMessage = "Use pelo menos 6 caracteres.")]
    public string Username { get; set; } = string.Empty;

    [Required(ErrorMessage = "Informe a senha.")]
    [MinLength(6, ErrorMessage = "Use pelo menos 6 caracteres.")]
    public string Password { get; set; } = string.Empty;
}

public sealed class RegisterFormModel
{
    [Required(ErrorMessage = "Informe o usuario.")]
    [MinLength(6, ErrorMessage = "Use pelo menos 6 caracteres.")]
    public string Username { get; set; } = string.Empty;

    [Required(ErrorMessage = "Informe o e-mail.")]
    [EmailAddress(ErrorMessage = "E-mail invalido.")]
    public string Email { get; set; } = string.Empty;

    [Required(ErrorMessage = "Informe o Discord ID.")]
    public string DiscordId { get; set; } = string.Empty;

    [Required(ErrorMessage = "Informe a senha.")]
    [MinLength(6, ErrorMessage = "Use pelo menos 6 caracteres.")]
    public string Password { get; set; } = string.Empty;

    [Compare(nameof(Password), ErrorMessage = "As senhas nao conferem.")]
    public string ConfirmPassword { get; set; } = string.Empty;
}

public sealed record ApiResult(bool Success, string Message);

public sealed record LoginResult(bool Success, string Message, string? Username = null, string? AccessLevel = null);
