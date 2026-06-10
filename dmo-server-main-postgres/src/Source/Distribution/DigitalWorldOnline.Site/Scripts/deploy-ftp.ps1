param(
    [string]$HostName = $env:LKDMO_FTP_HOST,
    [string]$UserName = $env:LKDMO_FTP_USER,
    [string]$Password = $env:LKDMO_FTP_PASSWORD,
    [int]$Port = $(if ($env:LKDMO_FTP_PORT) { [int]$env:LKDMO_FTP_PORT } else { 21 }),
    [string]$RemotePath = $(if ($env:LKDMO_FTP_REMOTE_PATH) { $env:LKDMO_FTP_REMOTE_PATH } else { "" }),
    [string]$LocalPath = "",
    [switch]$SkipPublish
)

$ErrorActionPreference = "Stop"

$projectRoot = Resolve-Path (Join-Path $PSScriptRoot "..")

if ([string]::IsNullOrWhiteSpace($HostName) -or
    [string]::IsNullOrWhiteSpace($UserName) -or
    [string]::IsNullOrWhiteSpace($Password)) {
    throw "Configure LKDMO_FTP_HOST, LKDMO_FTP_USER e LKDMO_FTP_PASSWORD antes do deploy."
}

if ([string]::IsNullOrWhiteSpace($LocalPath)) {
    $publishRoot = Join-Path $projectRoot "bin\Release\net8.0\publish"
    $LocalPath = Join-Path $publishRoot "wwwroot"
}

if (-not $SkipPublish) {
    & (Join-Path $PSScriptRoot "publish-site.ps1") -Environment Production
}

if (-not (Test-Path -LiteralPath $LocalPath)) {
    throw "Pasta local nao encontrada: $LocalPath"
}

function Convert-ToFtpPath {
    param([string]$Path)

    return ($Path -replace "\\", "/").Trim("/")
}

function New-FtpRequest {
    param(
        [string]$Path,
        [string]$Method
    )

    $normalizedPath = Convert-ToFtpPath $Path
    $uri = "ftp://${HostName}:$Port/$normalizedPath"
    $request = [System.Net.FtpWebRequest]::Create($uri)
    $request.Method = $Method
    $request.Credentials = New-Object System.Net.NetworkCredential($UserName, $Password)
    $request.UseBinary = $true
    $request.UsePassive = $true
    $request.KeepAlive = $false

    return $request
}

function Ensure-FtpDirectory {
    param([string]$Path)

    $parts = (Convert-ToFtpPath $Path).Split("/", [System.StringSplitOptions]::RemoveEmptyEntries)
    $current = ""

    foreach ($part in $parts) {
        $current = if ([string]::IsNullOrEmpty($current)) { $part } else { "$current/$part" }

        try {
            $request = New-FtpRequest -Path $current -Method ([System.Net.WebRequestMethods+Ftp]::MakeDirectory)
            $response = $request.GetResponse()
            $response.Close()
        }
        catch [System.Net.WebException] {
            if ($_.Exception.Response -ne $null) {
                $_.Exception.Response.Close()
            }
        }
    }
}

function Send-FtpFile {
    param(
        [string]$LocalFile,
        [string]$RemoteFile
    )

    $directory = Split-Path -Parent (Convert-ToFtpPath $RemoteFile)
    if (-not [string]::IsNullOrWhiteSpace($directory)) {
        Ensure-FtpDirectory -Path $directory
    }

    $bytes = [System.IO.File]::ReadAllBytes($LocalFile)
    $request = New-FtpRequest -Path $RemoteFile -Method ([System.Net.WebRequestMethods+Ftp]::UploadFile)
    $request.ContentLength = $bytes.Length

    $stream = $request.GetRequestStream()
    $stream.Write($bytes, 0, $bytes.Length)
    $stream.Close()

    $response = $request.GetResponse()
    $response.Close()
}

function Get-RelativeFilePath {
    param(
        [string]$RootPath,
        [string]$FilePath
    )

    $root = [System.IO.Path]::GetFullPath($RootPath).TrimEnd('\', '/') + [System.IO.Path]::DirectorySeparatorChar
    $file = [System.IO.Path]::GetFullPath($FilePath)

    if (-not $file.StartsWith($root, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Arquivo fora da pasta local: $FilePath"
    }

    return $file.Substring($root.Length)
}

$localRoot = Resolve-Path $LocalPath
$files = Get-ChildItem -LiteralPath $localRoot -File -Recurse

Write-Host "Enviando $($files.Count) arquivos para ftp://${HostName}:$Port/$RemotePath ..."

foreach ($file in $files) {
    $relativePath = Get-RelativeFilePath -RootPath $localRoot -FilePath $file.FullName
    $remoteFile = if ([string]::IsNullOrWhiteSpace($RemotePath)) {
        Convert-ToFtpPath $relativePath
    } else {
        Convert-ToFtpPath (Join-Path $RemotePath $relativePath)
    }

    Send-FtpFile -LocalFile $file.FullName -RemoteFile $remoteFile
}

Write-Host "Deploy FTP concluido."
