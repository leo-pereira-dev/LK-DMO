param(
    [ValidateSet("Development", "Production")]
    [string]$Environment = "Production",

    [string]$OutputPath = ""
)

$ErrorActionPreference = "Stop"

$projectRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$projectFile = Join-Path $projectRoot "DigitalWorldOnline.Site.csproj"
$publishDir = if ([string]::IsNullOrWhiteSpace($OutputPath)) {
    Join-Path $projectRoot "bin\Release\net8.0\publish"
} else {
    $OutputPath
}

if ($Environment -eq "Production") {
    $apiBaseUrl = $env:LKDMO_SITE_API_BASE_URL

    if ([string]::IsNullOrWhiteSpace($apiBaseUrl)) {
        $apiBaseUrl = "https://lkdmo.com.br/api/"
    }

    if (-not $apiBaseUrl.EndsWith("/")) {
        $apiBaseUrl += "/"
    }

    $productionSettingsPath = Join-Path $projectRoot "wwwroot\appsettings.Production.json"
    $productionSettings = [ordered]@{
        Api = [ordered]@{
            BaseUrl = $apiBaseUrl
            Token = ""
        }
    }

    $productionSettings |
        ConvertTo-Json -Depth 5 |
        Set-Content -LiteralPath $productionSettingsPath -Encoding UTF8
}

dotnet publish $projectFile -c Release -o $publishDir /p:BlazorEnvironment=$Environment

Write-Host "Publicado em: $publishDir"
Write-Host "Ambiente: $Environment"
