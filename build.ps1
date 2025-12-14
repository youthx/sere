$ErrorActionPreference = "Stop"

Write-Host "=== Building all modules ==="

# --- Build Engine first ---
$engineDir = ".\engine"
$engineScript = "build.ps1"
if (Test-Path (Join-Path $engineDir $engineScript)) {
    Write-Host ">>> Entering $engineDir"
    Push-Location $engineDir
    & .\$engineScript
    $exitCode = $LASTEXITCODE
    Pop-Location
    if ($exitCode -ne 0) {
        Write-Error "Engine build failed with exit code $exitCode"
        exit $exitCode
    }
} else {
    Write-Warning "Build script not found: $engineDir\$engineScript"
    exit 1
}

# --- Build Testbed next ---
$testbedDir = ".\testbed"
$testbedScript = "build.ps1"
if (Test-Path (Join-Path $testbedDir $testbedScript)) {
    Write-Host ">>> Entering $testbedDir"
    Push-Location $testbedDir
    & .\$testbedScript
    $exitCode = $LASTEXITCODE
    Pop-Location
    if ($exitCode -ne 0) {
        Write-Error "Testbed build failed with exit code $exitCode"
        exit $exitCode
    }
} else {
    Write-Warning "Build script not found: $testbedDir\$testbedScript"
    exit 1
}

clear 

./bin/testbed.exe
