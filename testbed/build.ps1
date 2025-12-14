# build_testbed.ps1
$ErrorActionPreference = "Stop"

# Settings
$assembly = "testbed"

# Compiler flags
$compilerFlags = @("-g")

# Include paths
$includeFlags = @("-Isrc", "-I../engine/src")

# Linker flags (use full path to .lib if needed)
$linkerFlags = @("-L../bin", "-lengine")  # clang on Windows finds engine.lib automatically

# Defines
$defines = @("-D_DEBUG", "-DKIMPORT")

# Collect all .c files recursively
$cFiles = Get-ChildItem -Path . -Recurse -Filter *.c | ForEach-Object { $_.FullName }

if ($cFiles.Count -eq 0) {
    Write-Host "No C files found!"
    exit
}

Write-Host "Building $assembly.exe with the following files:"
$cFiles | ForEach-Object { Write-Host "  $_" }

# Ensure output directory exists
$binDir = Join-Path ".." "bin"
if (!(Test-Path $binDir)) {
    New-Item -ItemType Directory -Path $binDir | Out-Null
}

# Build argument array
$clangArgs = @()
$clangArgs += $cFiles
$clangArgs += $compilerFlags
$clangArgs += $defines
$clangArgs += $includeFlags
$clangArgs += $linkerFlags
$clangArgs += "-o"
$clangArgs += Join-Path $binDir "$assembly.exe"

# Show command for debugging
Write-Host "Running clang with arguments:"
Write-Host ($clangArgs -join " ")

# Run clang
clang @clangArgs

Write-Host "Build finished!"
