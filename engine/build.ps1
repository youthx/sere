# build.ps1
$ErrorActionPreference = "Stop"

# Settings
$assembly = "engine"
$compilerFlags = @("-g","-shared","-Wvarargs")
$includeFlags = @("-Isrc", "-II:/Vulkan/Include")
$linkerFlags  = @("-luser32", "-lvulkan-1", "-LI:/Vulkan/Lib")
$defines = @("-D_DEBUG","-DKEXPORT","-D_CRT_SECURE_NO_WARNINGS")

# Collect all C files recursively
$cFiles = Get-ChildItem -Path . -Recurse -Filter *.c | ForEach-Object { $_.FullName }

if ($cFiles.Count -eq 0) {
    Write-Host "No C files found!"
    exit
}

Write-Host "Building $assembly.dll with the following files:"
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
$clangArgs += Join-Path $binDir "$assembly.dll"

# Show command (for debugging)
Write-Host "Running clang with arguments:"
Write-Host ($clangArgs -join " ")

# Execute clang
gcc @clangArgs

Write-Host "Build finished!"
