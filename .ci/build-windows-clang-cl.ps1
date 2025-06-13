# .ci/build-windows-clang-cl.ps1
# Enable strict error handling
$ErrorActionPreference = "Stop"

trap {
    Write-Host "ERROR: $($_.Exception.Message)"
    exit 1
}

Write-Host "Starting RPCS3 build (PowerShell script)"

Write-Host "Listing contents of vcpkg x64-windows directory"
$VcpkgRoot="$(Get-Location)/vcpkg"
Get-ChildItem -Path "$VcpkgRoot/installed/x64-windows/"

# Configure git safe directory
Write-Host "Configuring git safe directory"
& git config --global --add safe.directory '*'

# Initialize submodules except certain ones
Write-Host "Initializing submodules"
$excludedSubs = @('llvm','opencv','ffmpeg','FAudio')

# Get submodule paths excluding those in $excludedSubs
$submodules = Select-String -Path .gitmodules -Pattern 'path = (.+)' | ForEach-Object {
    $_.Matches[0].Groups[1].Value
} | Where-Object {
    $path = $_
    -not ($excludedSubs | Where-Object { $path -like "*$_*" })
}

Write-Host "Updating submodules: $($submodules -join ', ')"
& git submodule update --init --quiet $submodules

# Create and enter build directory
Write-Host "Creating build directory"
if (!(Test-Path build)) {
    New-Item -ItemType Directory -Path build | Out-Null
}
Set-Location build
Write-Host "Changed directory to: $(Get-Location)"

# Set environment variables for compiler and linker
Write-Host "Setting compiler and linker environment variables"
$env:CC = "clang-cl"
$env:CXX = "clang-cl"

if ($env:LLVMVER) {
    $env:AR = "llvm-ar-$env:LLVMVER"
    $env:RANLIB = "llvm-ranlib-$env:LLVMVER"
} else {
    $env:AR = "llvm-ar"
    $env:RANLIB = "llvm-ranlib"
}

# Run CMake with Ninja generator and required flags
Write-Host "Running CMake configuration"
& cmake .. `
    -G Ninja `
    -DCMAKE_C_COMPILER=clang-cl `
    -DCMAKE_CXX_COMPILER=clang-cl `
    -DCMAKE_INSTALL_PREFIX=/usr `
    -DUSE_NATIVE_INSTRUCTIONS=OFF `
    -DUSE_PRECOMPILED_HEADERS=OFF `
    -DCMAKE_AR=$env:AR `
    -DCMAKE_RANLIB=$env:RANLIB `
    -DCMAKE_TOOLCHAIN_FILE="$VcpkgRoot/scripts/buildsystems/vcpkg.cmake" `
    -DFFMPEG_INCLUDE_DIR="$VcpkgRoot/installed/x64-windows/include" `
    -DFFMPEG_LIBAVCODEC="$VcpkgRoot/installed/x64-windows/lib/avcodec.lib" `
    -DFFMPEG_LIBAVFORMAT="$VcpkgRoot/installed/x64-windows/lib/avformat.lib" `
    -DFFMPEG_LIBAVUTIL="$VcpkgRoot/installed/x64-windows/lib/avutil.lib" `
    -DFFMPEG_LIBSWSCALE="$VcpkgRoot/installed/x64-windows/lib/swscale.lib" `
    -DFFMPEG_LIBSWRESAMPLE="$VcpkgRoot/installed/x64-windows/lib/swresample.lib" `
    -DUSE_SYSTEM_CURL=OFF `
    -DUSE_FAUDIO=OFF `
    -DUSE_SDL=ON `
    -DUSE_SYSTEM_ZLIB=OFF `
    -DUSE_SYSTEM_SDL=OFF `
    -DUSE_SYSTEM_FFMPEG=ON `
    -DUSE_SYSTEM_OPENCV=ON `
    -DUSE_SYSTEM_OPENAL=OFF `
    -DUSE_DISCORD_RPC=ON `
    -DOpenGL_GL_PREFERENCE=LEGACY `
    -DWITH_LLVM=OFF `
    -DLLVM_DIR="$VcpkgRoot/installed/x64-windows/share/llvm" `
    -DSTATIC_LINK_LLVM=ON `
    -DBUILD_RPCS3_TESTS=OFF `
    -DRUN_RPCS3_TESTS=OFF
Write-Host "CMake configuration complete"

# Build with ninja
Write-Host "Starting build with Ninja..."
$ninjaExitCode = & ninja
if ($ninjaExitCode -ne 0) {
    Write-Host "Build failed with exit code $ninjaExitCode"
    exit 1
}

Write-Host "Build succeeded"

# Go back to root directory
Set-Location ..
Write-Host "Returned to root directory: $(Get-Location)"

# Deploy if build succeeded
Write-Host "Running deployment script"
& .ci/deploy-windows-clang-cl.sh x86_64
