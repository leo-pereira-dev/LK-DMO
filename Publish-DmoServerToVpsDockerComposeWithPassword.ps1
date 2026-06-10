param(
    [string]$HostName = "104.234.41.94",
    [string]$UserName = "ubuntu",
    [string]$PasswordFile = (Join-Path $PSScriptRoot ".codex-tmp\dmo-vps-password.txt"),
    [string]$LocalSource = (Join-Path $PSScriptRoot "dmo-server-main-postgres"),
    [string]$RemoteRoot = "/home/ubuntu/dmo-server-main-postgres",
    [string]$ComposeEnv = ".env.production",
    [string[]]$Services = @("account", "character", "game", "routine", "api", "admin"),
    [switch]$NoBuild,
    [switch]$ReuseLatestArchive,
    [switch]$CheckConnection
)

# Use this wrapper for VPS deploys from this machine when SSH keys are not
# configured. Plain ssh/scp automation waits for an interactive password prompt
# and looks like a hung deploy. This wrapper uses bundled Python + Paramiko and
# reads the password from a local-only file under .codex-tmp.
#
# This deploy intentionally targets app services only by default and does not
# recreate postgres, because the production database already lives on the VPS.

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$python = "C:\Users\leo_h\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe"
$helper = Join-Path $PSScriptRoot "Publish-DmoServerToVpsDockerComposeWithPassword.py"

if (-not (Test-Path -LiteralPath $python -PathType Leaf)) {
    throw "Bundled Python not found: $python"
}

if (-not (Test-Path -LiteralPath $helper -PathType Leaf)) {
    throw "Python deploy helper not found: $helper"
}

if (-not (Test-Path -LiteralPath $PasswordFile -PathType Leaf)) {
    $passwordDir = Split-Path -Parent $PasswordFile
    New-Item -ItemType Directory -Force -Path $passwordDir | Out-Null
    Set-Content -LiteralPath $PasswordFile -Value "COLE_A_SENHA_AQUI" -Encoding ascii
    throw "Password file created. Put the VPS password in it, save, then run again: $PasswordFile"
}

$arguments = @(
    $helper,
    "--host", $HostName,
    "--user", $UserName,
    "--password-file", $PasswordFile,
    "--local-source", $LocalSource,
    "--remote-root", $RemoteRoot,
    "--compose-env", $ComposeEnv
)

foreach ($service in $Services) {
    $arguments += @("--service", $service)
}

if ($NoBuild) {
    $arguments += "--no-build"
}

if ($ReuseLatestArchive) {
    $arguments += "--reuse-latest-archive"
}

if ($CheckConnection) {
    $arguments += "--check-connection"
}

& $python @arguments
if ($LASTEXITCODE -ne 0) {
    throw "Deploy failed with exit code $LASTEXITCODE."
}
