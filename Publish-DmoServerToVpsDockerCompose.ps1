param(
    [string]$HostName = "104.234.41.94",
    [string]$UserName = "ubuntu",
    [string]$LocalSource = (Join-Path $PSScriptRoot "dmo-server-main-postgres"),
    [string]$RemoteRoot = "/home/ubuntu/dmo-server-main-postgres",
    [string]$ComposeEnv = ".env.production",
    [string[]]$Services = @("account", "character", "game"),
    [string]$IdentityFile = "",
    [switch]$AppServices,
    [switch]$AllServices,
    [switch]$SkipUpload,
    [switch]$NoBuild,
    [switch]$InteractiveSsh
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Invoke-Checked {
    param(
        [Parameter(Mandatory = $true)]
        [string]$FilePath,

        [Parameter(Mandatory = $true)]
        [string[]]$Arguments
    )

    Write-Host ">> $FilePath $($Arguments -join ' ')"
    & $FilePath @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed with exit code ${LASTEXITCODE}: $FilePath"
    }
}

function Assert-InWorkspace {
    param([Parameter(Mandatory = $true)][string]$Path)

    $workspace = (Resolve-Path -LiteralPath $PSScriptRoot).Path.TrimEnd('\')
    $resolved = (Resolve-Path -LiteralPath $Path).Path
    if (-not $resolved.StartsWith($workspace, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to publish a source outside this workspace: $resolved"
    }
}

if (-not (Test-Path -LiteralPath $LocalSource -PathType Container)) {
    throw "Local source folder not found: $LocalSource"
}

Assert-InWorkspace -Path $LocalSource

$composeFile = Join-Path $LocalSource "docker-compose.yml"
if (-not (Test-Path -LiteralPath $composeFile -PathType Leaf)) {
    throw "docker-compose.yml not found in local source: $composeFile"
}

$ssh = Get-Command ssh -ErrorAction Stop
$scp = Get-Command scp -ErrorAction Stop
$tar = Get-Command tar -ErrorAction Stop

$stamp = Get-Date -Format "yyyyMMdd-HHmmss"
$tmpRoot = Join-Path $PSScriptRoot ".codex-tmp"
New-Item -ItemType Directory -Force -Path $tmpRoot | Out-Null

$archiveName = "dmo-server-main-postgres-$stamp.tar.gz"
$archivePath = Join-Path $tmpRoot $archiveName
$remoteScriptName = "publish-dmo-server-$stamp.sh"
$remoteScriptPath = Join-Path $tmpRoot $remoteScriptName
$remoteArchive = "/tmp/$archiveName"
$remoteScript = "/tmp/$remoteScriptName"
$target = "${UserName}@${HostName}"
$sshCommonArgs = @()
if (-not $InteractiveSsh) {
    $sshCommonArgs = @("-o", "BatchMode=yes", "-o", "ConnectTimeout=15")
}
if (-not [string]::IsNullOrWhiteSpace($IdentityFile)) {
    $sshCommonArgs += @("-i", $IdentityFile)
}

if (-not $SkipUpload) {
    if (Test-Path -LiteralPath $archivePath) {
        Remove-Item -LiteralPath $archivePath -Force
    }

    $tarArgs = @(
        "-czf", $archivePath,
        "--exclude=.git",
        "--exclude=.vs",
        "--exclude=.codex-tmp",
        "--exclude=logs",
        "--exclude=deploy-backups",
        "-C", $LocalSource,
        "."
    )

    Invoke-Checked -FilePath $tar.Source -Arguments $tarArgs
}

$remoteDeployScript = @'
#!/usr/bin/env bash
set -euo pipefail

REMOTE_ROOT="$1"
ARCHIVE="$2"
ENV_FILE="$3"
BUILD_MODE="$4"
shift 4
SERVICES=("$@")

STAMP="$(date +%Y%m%d-%H%M%S)"
REMOTE_PARENT="$(dirname "$REMOTE_ROOT")"
TMP_ROOT="${REMOTE_ROOT}.deploy-${STAMP}"
BACKUP_ROOT="${REMOTE_PARENT}/deploy-backups/dmo-server-main-postgres-${STAMP}"

if [ ! -f "$ARCHIVE" ]; then
  echo "Archive not found: $ARCHIVE" >&2
  exit 1
fi

rm -rf "$TMP_ROOT"
mkdir -p "$TMP_ROOT"
tar -xzf "$ARCHIVE" -C "$TMP_ROOT"

if [ -f "${REMOTE_ROOT}/${ENV_FILE}" ] && [ ! -f "${TMP_ROOT}/${ENV_FILE}" ]; then
  cp "${REMOTE_ROOT}/${ENV_FILE}" "${TMP_ROOT}/${ENV_FILE}"
fi

if [ ! -f "${TMP_ROOT}/${ENV_FILE}" ]; then
  echo "Required compose env file not found after staging: ${TMP_ROOT}/${ENV_FILE}" >&2
  echo "Create it on the VPS before publishing, or include a safe deployment copy." >&2
  exit 1
fi

mkdir -p "${REMOTE_PARENT}/deploy-backups"

if [ -d "$REMOTE_ROOT" ]; then
  mv "$REMOTE_ROOT" "$BACKUP_ROOT"
fi

mv "$TMP_ROOT" "$REMOTE_ROOT"
cd "$REMOTE_ROOT"

if [ "$BUILD_MODE" = "build" ]; then
  COMPOSE_ARGS=(up -d --build)
else
  COMPOSE_ARGS=(up -d)
fi

if [ "${#SERVICES[@]}" -gt 0 ]; then
  sudo docker compose --env-file "$ENV_FILE" "${COMPOSE_ARGS[@]}" "${SERVICES[@]}"
else
  sudo docker compose --env-file "$ENV_FILE" "${COMPOSE_ARGS[@]}"
fi

sudo docker compose --env-file "$ENV_FILE" ps

rm -f "$ARCHIVE" "$0"
echo "Backup kept at: $BACKUP_ROOT"
'@

$remoteDeployScriptLf = $remoteDeployScript -replace "`r?`n", "`n"
[System.IO.File]::WriteAllText($remoteScriptPath, $remoteDeployScriptLf, [System.Text.Encoding]::ASCII)

if (-not $SkipUpload) {
    Invoke-Checked -FilePath $scp.Source -Arguments ($sshCommonArgs + @($archivePath, $remoteScriptPath, "${target}:/tmp/"))
} else {
    Invoke-Checked -FilePath $scp.Source -Arguments ($sshCommonArgs + @($remoteScriptPath, "${target}:/tmp/"))
}

$serviceArgs = @()
if ($AppServices) {
    $serviceArgs = @("account", "character", "game", "routine", "api", "admin")
} elseif (-not $AllServices) {
    $serviceArgs = $Services
}

$buildMode = if ($NoBuild) { "nobuild" } else { "build" }
$sshArgs = $sshCommonArgs + @(
    $target,
    "bash",
    $remoteScript,
    $RemoteRoot,
    $remoteArchive,
    $ComposeEnv,
    $buildMode
) + $serviceArgs

Invoke-Checked -FilePath $ssh.Source -Arguments $sshArgs

if (Test-Path -LiteralPath $remoteScriptPath) {
    Remove-Item -LiteralPath $remoteScriptPath -Force
}

Write-Host "Publish complete."
