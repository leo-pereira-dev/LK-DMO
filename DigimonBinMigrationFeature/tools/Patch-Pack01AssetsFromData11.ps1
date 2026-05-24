param(
    [string]$PackBasePath = "C:\0-NewDmo\ClientDist\Data\Pack01",
    [string]$SourceRoot = "D:\LK-DMO TOOLS\EXTRATOR-Copiar Daqui\Decryptorv2\data11",
    [string]$ReportDir = "C:\0-NewDmo\DigimonBinMigrationFeature\generated\reports",
    [string[]]$ForceUpdateRelativeFiles = @("data\model.dat"),
    [switch]$PatchAllMissing
)

$ErrorActionPreference = "Stop"

function Get-PackHash([string]$path) {
    $lower = $path.ToLowerInvariant()
    $mask = [uint64]4294967295
    $result = [uint64]5381
    foreach ($ch in $lower.ToCharArray()) {
        if ($ch -eq '.' -or $ch -eq '\') {
            continue
        }
        $result = (($result -shl 5) + $result + [byte][char]$ch) -band $mask
    }
    return [uint32]$result
}

function Read-UInt32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToUInt32($bytes, $offset)
}

function Read-Int32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToInt32($bytes, $offset)
}

function Read-UInt64LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToUInt64($bytes, $offset)
}

function Write-ChunkHeader([IO.BinaryWriter]$writer, [string]$relativePath) {
    $pathBytes = [Text.Encoding]::ASCII.GetBytes($relativePath)
    if ($pathBytes.Length -gt 259) {
        throw "Pack path is too long: $relativePath"
    }

    $buffer = New-Object byte[] 260
    for ($i = 0; $i -lt $pathBytes.Length; $i++) {
        $buffer[$i] = $pathBytes[$i] -bxor 0xd0
    }

    $writer.Write($buffer)
    $writer.Write([uint32]0)
    $writer.Write([int]$pathBytes.Length)
}

function Write-HashFile([string]$hashPath, [object[]]$entries) {
    $fs = [IO.File]::Open($hashPath, [IO.FileMode]::Create, [IO.FileAccess]::Write)
    $writer = New-Object IO.BinaryWriter($fs)
    try {
        $writer.Write([uint32]0x10)
        $writer.Write([int]$entries.Count)
        foreach ($entry in ($entries | Sort-Object Hash)) {
            $writer.Write([int]$entry.Plag)
            $writer.Write([uint32]$entry.DataSize)
            $writer.Write([uint32]$entry.AllocSize)
            $writer.Write([uint32]$entry.Hash)
            $writer.Write([uint64]$entry.Offset)
        }
    }
    finally {
        $writer.Dispose()
        $fs.Dispose()
    }
}

function Get-SourceFileForPackPath([string]$packRelativePath) {
    $normalized = $packRelativePath.Replace('/', '\')
    if ($normalized.StartsWith("data\", [StringComparison]::OrdinalIgnoreCase)) {
        $normalized = $normalized.Substring(5)
    }

    return Join-Path $SourceRoot $normalized
}

function Add-PatchCandidate([System.Collections.Generic.List[object]]$list, [string]$packPath, [string]$sourcePath, [string]$reason) {
    $item = Get-Item -LiteralPath $sourcePath
    $hash = Get-PackHash $packPath
    $key = $packPath.ToLowerInvariant()
    if ($script:candidateKeys.ContainsKey($key)) {
        return
    }

    $script:candidateKeys[$key] = $true
    $list.Add([pscustomobject]@{
        Path = $packPath
        Source = $item.FullName
        Size = [uint64]$item.Length
        Hash = [uint32]$hash
        HashText = ("0x{0:X8}" -f $hash)
        Reason = $reason
    })
}

$pfPath = "$PackBasePath.pf"
$hfPath = "$PackBasePath.hf"
if (-not (Test-Path -LiteralPath $pfPath) -or -not (Test-Path -LiteralPath $hfPath)) {
    throw "Pack files not found for base path: $PackBasePath"
}

if (-not (Test-Path -LiteralPath $SourceRoot)) {
    throw "Source root not found: $SourceRoot"
}

New-Item -ItemType Directory -Force -Path $ReportDir | Out-Null

$hashBytes = [IO.File]::ReadAllBytes($hfPath)
$version = Read-UInt32LE $hashBytes 0
if ($version -ne 0x10) {
    throw "Unsupported hash version 0x$($version.ToString('X')) in $hfPath"
}

$count = Read-Int32LE $hashBytes 4
$entriesByHash = @{}
for ($i = 0; $i -lt $count; $i++) {
    $offset = 8 + ($i * 24)
    $entry = [pscustomobject]@{
        Plag = Read-Int32LE $hashBytes $offset
        DataSize = Read-UInt32LE $hashBytes ($offset + 4)
        AllocSize = Read-UInt32LE $hashBytes ($offset + 8)
        Hash = Read-UInt32LE $hashBytes ($offset + 12)
        Offset = Read-UInt64LE $hashBytes ($offset + 16)
    }
    $entriesByHash[[uint32]$entry.Hash] = $entry
}

$candidates = New-Object System.Collections.Generic.List[object]
$script:candidateKeys = @{}

foreach ($relative in $ForceUpdateRelativeFiles) {
    $packPath = $relative.Replace('/', '\')
    $sourcePath = Get-SourceFileForPackPath $packPath
    if (-not (Test-Path -LiteralPath $sourcePath)) {
        throw "Forced source file not found: $sourcePath"
    }

    Add-PatchCandidate $candidates $packPath $sourcePath "forced-update"
}

$sourceFiles = Get-ChildItem -LiteralPath $SourceRoot -Recurse -File -Force
$missingReport = New-Object System.Collections.Generic.List[object]
foreach ($file in $sourceFiles) {
    $relative = $file.FullName.Substring($SourceRoot.Length).TrimStart('\')
    $packPath = "data\" + $relative
    $hash = Get-PackHash $packPath
    if (-not $entriesByHash.ContainsKey($hash)) {
        $row = [pscustomobject]@{
            Path = $packPath
            Source = $file.FullName
            Size = [uint64]$file.Length
            Hash = ("0x{0:X8}" -f $hash)
        }
        $missingReport.Add($row)
        if ($PatchAllMissing) {
            Add-PatchCandidate $candidates $packPath $file.FullName "missing"
        }
    }
}

$stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$missingCsv = Join-Path $ReportDir "pack01-missing-assets-$stamp.csv"
$patchedCsv = Join-Path $ReportDir "pack01-patched-assets-$stamp.csv"
$restoreInfo = Join-Path $ReportDir "pack01-restore-info-$stamp.txt"
$missingReport | Export-Csv -LiteralPath $missingCsv -NoTypeInformation -Encoding UTF8

$pfItem = Get-Item -LiteralPath $pfPath
$hfBackup = "$hfPath.bak_assets_$stamp"
Copy-Item -LiteralPath $hfPath -Destination $hfBackup -Force
@(
    "PackBasePath=$PackBasePath"
    "OriginalPfLength=$($pfItem.Length)"
    "OriginalHfBackup=$hfBackup"
    "CreatedAt=$stamp"
) | Set-Content -LiteralPath $restoreInfo -Encoding ASCII

$pfStream = [IO.File]::Open($pfPath, [IO.FileMode]::Open, [IO.FileAccess]::ReadWrite, [IO.FileShare]::None)
$writer = New-Object IO.BinaryWriter($pfStream)
$patched = New-Object System.Collections.Generic.List[object]
try {
    foreach ($candidate in $candidates) {
        if ($candidate.Size -gt [uint32]::MaxValue) {
            throw "File too large for pack entry: $($candidate.Source)"
        }

        $pfStream.Seek(0, [IO.SeekOrigin]::End) | Out-Null
        $chunkOffset = [uint64]$pfStream.Position
        Write-ChunkHeader $writer $candidate.Path
        $dataOffset = [uint64]$pfStream.Position

        $sourceStream = [IO.File]::Open($candidate.Source, [IO.FileMode]::Open, [IO.FileAccess]::Read, [IO.FileShare]::Read)
        try {
            $sourceStream.CopyTo($pfStream)
        }
        finally {
            $sourceStream.Dispose()
        }

        if ($entriesByHash.ContainsKey($candidate.Hash)) {
            $entriesByHash[$candidate.Hash].DataSize = [uint32]$candidate.Size
            $entriesByHash[$candidate.Hash].AllocSize = [uint32]$candidate.Size
            $entriesByHash[$candidate.Hash].Offset = $dataOffset
        }
        else {
            $entriesByHash[$candidate.Hash] = [pscustomobject]@{
                Plag = 1
                DataSize = [uint32]$candidate.Size
                AllocSize = [uint32]$candidate.Size
                Hash = [uint32]$candidate.Hash
                Offset = $dataOffset
            }
        }

        $patched.Add([pscustomobject]@{
            Path = $candidate.Path
            Hash = $candidate.HashText
            Size = $candidate.Size
            Reason = $candidate.Reason
            ChunkOffset = $chunkOffset
            DataOffset = $dataOffset
        })
    }
}
finally {
    $writer.Dispose()
    $pfStream.Dispose()
}

Write-HashFile $hfPath @($entriesByHash.Values)
$patched | Export-Csv -LiteralPath $patchedCsv -NoTypeInformation -Encoding UTF8

$summary = [pscustomobject]@{
    Pack = $PackBasePath
    SourceRoot = $SourceRoot
    OriginalEntries = $count
    NewEntries = $entriesByHash.Count
    MissingDetected = $missingReport.Count
    Patched = $patched.Count
    MissingReport = $missingCsv
    PatchedReport = $patchedCsv
    RestoreInfo = $restoreInfo
    HfBackup = $hfBackup
}

$summary | Format-List
$patched | Select-Object -First 30 | Format-Table -AutoSize
if ($patched.Count -gt 30) {
    Write-Host "..."
    Write-Host "Patched list truncated in console; full CSV: $patchedCsv"
}
