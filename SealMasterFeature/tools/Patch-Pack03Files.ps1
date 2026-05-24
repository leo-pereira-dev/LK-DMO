param(
    [string]$PackBasePath = "C:\0-NewDmo\ClientDist\Data\Pack03",
    [string]$SourceBinDir = "C:\0-NewDmo\DigiCombineFeature\assets\extracted_pack03\data\bin\english",
    [string[]]$RelativeFiles = @(
        "data\bin\english\MasterCard.bin",
        "data\bin\english\SealBaseImage.bin",
        "data\bin\english\SealCard.bin",
        "data\bin\english\SealDigimonImage.bin",
        "data\bin\english\SealFilter.bin",
        "data\bin\english\SealLeader.bin",
        "data\bin\english\SealMasterName_Str.bin",
        "data\bin\english\SealNatureImage.bin",
        "data\bin\english\SealPlateImage.bin"
    )
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

function Write-Chunk([IO.BinaryWriter]$writer, [string]$relativePath) {
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

$pfPath = "$PackBasePath.pf"
$hfPath = "$PackBasePath.hf"
if (-not (Test-Path $pfPath) -or -not (Test-Path $hfPath)) {
    throw "Pack files not found for base path: $PackBasePath"
}

$stamp = Get-Date -Format "yyyyMMdd_HHmmss"
Copy-Item -Path $pfPath -Destination "$pfPath.bak_seals_$stamp" -Force
Copy-Item -Path $hfPath -Destination "$hfPath.bak_seals_$stamp" -Force

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

$pfStream = [IO.File]::Open($pfPath, [IO.FileMode]::Open, [IO.FileAccess]::ReadWrite, [IO.FileShare]::None)
$writer = New-Object IO.BinaryWriter($pfStream)
$patched = New-Object System.Collections.Generic.List[object]
try {
    foreach ($relative in $RelativeFiles) {
        $fileName = Split-Path $relative -Leaf
        $source = Join-Path $SourceBinDir $fileName
        if (-not (Test-Path $source)) {
            Write-Warning "Skipping missing source file: $source"
            continue
        }

        $data = [IO.File]::ReadAllBytes($source)
        $pfStream.Seek(0, [IO.SeekOrigin]::End) | Out-Null
        $chunkOffset = [uint64]$pfStream.Position
        Write-Chunk $writer $relative
        $dataOffset = [uint64]$pfStream.Position
        $writer.Write($data)

        $hash = Get-PackHash $relative
        if ($entriesByHash.ContainsKey($hash)) {
            $entriesByHash[$hash].DataSize = [uint32]$data.Length
            $entriesByHash[$hash].AllocSize = [uint32]$data.Length
            $entriesByHash[$hash].Offset = $dataOffset
        }
        else {
            $entriesByHash[$hash] = [pscustomobject]@{
                Plag = 1
                DataSize = [uint32]$data.Length
                AllocSize = [uint32]$data.Length
                Hash = [uint32]$hash
                Offset = $dataOffset
            }
        }

        $patched.Add([pscustomobject]@{
            Path = $relative
            Hash = ("0x{0:X8}" -f $hash)
            Size = $data.Length
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

$patched | Format-Table -AutoSize
Write-Host "Updated pack: $pfPath"
Write-Host "Updated hash: $hfPath"
Write-Host "File count: $($entriesByHash.Count)"
Write-Host "Backups: $pfPath.bak_seals_$stamp / $hfPath.bak_seals_$stamp"
