param(
    [string]$SourceTacticsBin = "C:\0-NewDmo\dmo-server-main-postgres\Bins\data\bin\english\Tactics.bin",
    [string]$DigimonReportCsv = "C:\0-NewDmo\DigimonBinMigrationFeature\generated\reports\new-digimon-list.csv",
    [string]$EvolutionBasesCsv = "C:\0-NewDmo\DigimonBinMigrationFeature\generated\reports\new-evolution-bases.csv",
    [string]$OutputTacticsBin = "C:\0-NewDmo\DigimonBinMigrationFeature\generated\converted\Tactics.bin",
    [string]$ReportCsv = "C:\0-NewDmo\DigimonBinMigrationFeature\generated\reports\tactics-explain-added.csv"
)

$ErrorActionPreference = "Stop"

function Read-UInt32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToUInt32($bytes, $offset)
}

function Read-Int32LE([byte[]]$bytes, [int]$offset) {
    [BitConverter]::ToInt32($bytes, $offset)
}

function Write-Int32LE([IO.BinaryWriter]$writer, [int]$value) {
    $writer.Write([BitConverter]::GetBytes([int]$value))
}

function Write-UInt32LE([IO.BinaryWriter]$writer, [uint32]$value) {
    $writer.Write([BitConverter]::GetBytes([uint32]$value))
}

function Write-WideFixed([IO.BinaryWriter]$writer, [string]$text, [int]$byteLength) {
    $buffer = New-Object byte[] $byteLength
    if (-not [string]::IsNullOrWhiteSpace($text)) {
        $encoded = [Text.Encoding]::Unicode.GetBytes($text)
        $copyLength = [Math]::Min($encoded.Length, $byteLength - 2)
        [Array]::Copy($encoded, 0, $buffer, 0, $copyLength)
    }
    $writer.Write($buffer)
}

if (-not (Test-Path $SourceTacticsBin)) {
    throw "Source Tactics.bin not found: $SourceTacticsBin"
}
if (-not (Test-Path $DigimonReportCsv)) {
    throw "Digimon report not found: $DigimonReportCsv"
}

$bytes = [IO.File]::ReadAllBytes($SourceTacticsBin)
$offset = 0

$tacticsCount = Read-Int32LE $bytes $offset
$offset += 4

$tacticsRecordSize = 24
$offset += $tacticsCount * $tacticsRecordSize
if ($offset + 4 -gt $bytes.Length) {
    throw "Invalid Tactics.bin: tactics section exceeds file length."
}

$explainCountOffset = $offset
$explainCount = Read-Int32LE $bytes $offset
$offset += 4

$explainRecordSize = 1160
$existingIds = New-Object 'System.Collections.Generic.HashSet[uint32]'
for ($i = 0; $i -lt $explainCount; $i++) {
    if ($offset + $explainRecordSize -gt $bytes.Length) {
        throw "Invalid Tactics.bin: explain section exceeds file length."
    }

    [void]$existingIds.Add((Read-UInt32LE $bytes $offset))
    $offset += $explainRecordSize
}

$afterExplainOffset = $offset
$rows = Import-Csv $DigimonReportCsv
$baseRows = @()
if (Test-Path $EvolutionBasesCsv) {
    $baseRows = Import-Csv $EvolutionBasesCsv
}

$namesById = @{}
foreach ($row in $rows) {
    $name = [string]$row.Name
    if ([string]::IsNullOrWhiteSpace($name) -or $name -eq "_" -or $name -eq "-") {
        continue
    }

    $digimonId = [uint32]$row.DigimonId
    if ($digimonId -ne 0 -and -not $namesById.ContainsKey($digimonId)) {
        $namesById[$digimonId] = $name.Trim()
    }

    $modelId = [uint32]$row.ModelId
    if ($modelId -ne 0 -and -not $namesById.ContainsKey($modelId)) {
        $namesById[$modelId] = $name.Trim()
    }
}

foreach ($row in $baseRows) {
    $baseId = [uint32]$row.BaseId
    $baseName = [string]$row.BaseName
    if ($baseId -ne 0 -and
        -not $namesById.ContainsKey($baseId) -and
        -not [string]::IsNullOrWhiteSpace($baseName) -and
        $baseName -ne "_" -and
        $baseName -ne "-") {
        $namesById[$baseId] = $baseName.Trim()
    }
}

$toAdd = New-Object System.Collections.Generic.List[object]

foreach ($id in ($namesById.Keys | Sort-Object)) {
    $name = [string]$namesById[$id]
    if ($id -eq 0 -or $existingIds.Contains($id)) {
        continue
    }

    [void]$existingIds.Add($id)
    $toAdd.Add([pscustomobject]@{
        DigimonId = $id
        Name = $name.Trim()
    })
}

$outDir = Split-Path $OutputTacticsBin -Parent
if (-not (Test-Path $outDir)) {
    New-Item -ItemType Directory -Path $outDir | Out-Null
}
$reportDir = Split-Path $ReportCsv -Parent
if (-not (Test-Path $reportDir)) {
    New-Item -ItemType Directory -Path $reportDir | Out-Null
}

$fs = [IO.File]::Open($OutputTacticsBin, [IO.FileMode]::Create, [IO.FileAccess]::Write)
$writer = New-Object IO.BinaryWriter($fs)
try {
    $writer.Write($bytes, 0, $explainCountOffset)
    Write-Int32LE $writer ($explainCount + $toAdd.Count)

    $existingExplainBytes = $afterExplainOffset - ($explainCountOffset + 4)
    $writer.Write($bytes, $explainCountOffset + 4, $existingExplainBytes)

    foreach ($entry in ($toAdd | Sort-Object DigimonId)) {
        Write-UInt32LE $writer ([uint32]$entry.DigimonId)
        Write-UInt32LE $writer ([uint32]$entry.DigimonId)
        Write-WideFixed $writer $entry.Name 128
        Write-WideFixed $writer "No tactics description available." 1024
    }

    $writer.Write($bytes, $afterExplainOffset, $bytes.Length - $afterExplainOffset)
}
finally {
    $writer.Dispose()
    $fs.Dispose()
}

$toAdd |
    Sort-Object DigimonId |
    Export-Csv -Path $ReportCsv -NoTypeInformation -Encoding UTF8

[pscustomobject]@{
    Source = $SourceTacticsBin
    Output = $OutputTacticsBin
    ExistingExplainCount = $explainCount
    AddedExplainCount = $toAdd.Count
    FinalExplainCount = $explainCount + $toAdd.Count
    Report = $ReportCsv
}
