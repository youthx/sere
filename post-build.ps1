# Ensure shaders output directory exists
$shaderDir = Join-Path $PWD "bin/assets/shaders"
if (-not (Test-Path $shaderDir)) {
    New-Item -ItemType Directory -Path $shaderDir | Out-Null
}

Write-Host "Compiling shaders..."

# Compile vertex shader
glslc -fshader-stage=vert "assets/shaders/Builtin.ObjectShader.vert.glsl" -o "bin/assets/shaders/Builtin.ObjectShader.vert.spv"

# Compile fragment shader
glslc -fshader-stage=frag "assets/shaders/Builtin.ObjectShader.frag.glsl" -o "bin/assets/shaders/Builtin.ObjectShader.frag.spv"

Write-Host "Copying assets..."

# Copy assets folder recursively with overwrite
Copy-Item (Join-Path $PWD "assets" "*") (Join-Path $PWD "bin/assets") -Recurse -Force
