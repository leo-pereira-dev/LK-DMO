using System.Net;
using System.Net.Http.Json;
using System.Text.Json;
using DigitalWorldOnline.Site.Models;
using Microsoft.AspNetCore.Components;
using Microsoft.Extensions.Configuration;

namespace DigitalWorldOnline.Site.Services;

public sealed class AccountApiClient
{
    private readonly HttpClient _httpClient;
    private readonly IConfiguration _configuration;
    private readonly NavigationManager _navigation;

    public AccountApiClient(HttpClient httpClient, IConfiguration configuration, NavigationManager navigation)
    {
        _httpClient = httpClient;
        _configuration = configuration;
        _navigation = navigation;
    }

    public async Task<LoginResult> LoginAsync(string username, string password)
    {
        var response = await _httpClient.PostAsJsonAsync(
            BuildApiUri("v1/Account/login"),
            new { Username = username, Password = password });

        if (response.IsSuccessStatusCode)
        {
            var payload = await response.Content.ReadFromJsonAsync<LoginPayload>();
            return new LoginResult(true, "Login realizado com sucesso.", payload?.Username, payload?.AccessLevel);
        }

        return new LoginResult(false, await ReadProblemAsync(response, "Usuario ou senha invalidos."));
    }

    public async Task<ApiResult> CreateAccountAsync(string username, string email, string discordId, string password)
    {
        using var request = new HttpRequestMessage(HttpMethod.Post, BuildApiUri("v1/Account/register"))
        {
            Content = JsonContent.Create(new
            {
                Username = username,
                Email = email,
                DiscordId = discordId,
                Password = password
            })
        };

        var response = await _httpClient.SendAsync(request);
        if (response.StatusCode == HttpStatusCode.Created)
        {
            return new ApiResult(true, "Conta criada com sucesso. Voce ja pode entrar.");
        }

        return new ApiResult(false, await ReadProblemAsync(response, "Nao foi possivel criar a conta."));
    }

    private Uri BuildApiUri(string path)
    {
        var baseUrl = _configuration["Api:BaseUrl"];
        if (string.IsNullOrWhiteSpace(baseUrl))
        {
            return new Uri(path, UriKind.Relative);
        }

        if (Uri.TryCreate(baseUrl, UriKind.Absolute, out var absoluteBaseUri))
        {
            return new Uri(absoluteBaseUri, path);
        }

        var normalizedBase = baseUrl.EndsWith('/') ? baseUrl : $"{baseUrl}/";
        var relativePath = $"{normalizedBase.TrimStart('/')}{path}";

        return _navigation.ToAbsoluteUri(relativePath);
    }

    private static async Task<string> ReadProblemAsync(HttpResponseMessage response, string fallback)
    {
        var text = await response.Content.ReadAsStringAsync();
        if (string.IsNullOrWhiteSpace(text))
        {
            return fallback;
        }

        try
        {
            using var document = JsonDocument.Parse(text);
            if (document.RootElement.TryGetProperty("detail", out var detail))
            {
                return detail.GetString() ?? fallback;
            }

            if (document.RootElement.TryGetProperty("message", out var message))
            {
                return message.GetString() ?? fallback;
            }

            if (document.RootElement.TryGetProperty("Message", out var messagePascal))
            {
                return messagePascal.GetString() ?? fallback;
            }
        }
        catch (JsonException)
        {
            return text;
        }

        return fallback;
    }

    private sealed record LoginPayload(string Username, string AccessLevel);
}
