
$SIZE = 1024 * 1024

# Find the Assignment9 root directory
$PROJECT_DIR = Split-Path -Parent $PSScriptRoot

$DATA_DIR = Join-Path $PROJECT_DIR "data"
$OUTPUT_FILE = Join-Path $DATA_DIR "windows_cng.bin"

# Create data directory if it doesn't exist
New-Item -ItemType Directory -Force -Path $DATA_DIR | Out-Null

# Create cryptographic random number generator
$rng = [System.Security.Cryptography.RandomNumberGenerator]::Create()

try {
    $randomData = New-Object byte[] $SIZE

    $rng.GetBytes($randomData)

    [System.IO.File]::WriteAllBytes(
        $OUTPUT_FILE,
        $randomData
    )

    Write-Host "Windows CNG random data generated successfully."
    Write-Host "File: $OUTPUT_FILE"
    Write-Host "Size: $($randomData.Length) bytes"
}
finally {
    $rng.Dispose()
}
