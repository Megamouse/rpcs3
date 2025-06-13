#!/bin/sh -ex

git config --global --add safe.directory '*'

# Generate env vars from vcvarsall.bat
cmd /c '"C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64 && set' > .ci/ms_env.txt

# Export those variables in the current shell
while IFS='=' read -r name value; do
  case "$name" in
    INCLUDE|LIB|LIBPATH|PATH|_CL_*)
      export "$name=$value"
      ;;
  esac
done < .ci/ms_env.txt

# source ms_env.txt
# shellcheck disable=SC1091
. .ci/ms_env.txt

# Pull all the submodules except some
# Note: Tried to use git submodule status, but it takes over 20 seconds
# shellcheck disable=SC2046
git submodule -q update --init $(awk '/path/ && !/llvm/ && !/opencv/ && !/ffmpeg/ && !/curl/ && !/FAudio/ { print $3 }' .gitmodules)

mkdir build && cd build || exit 1

export CC="clang-cl"
export CXX="clang-cl"
export LINKER=lld
export LINKER_FLAG="-fuse-ld=${LINKER}"

if [ -n "$LLVMVER" ]; then
  export AR="llvm-ar-$LLVMVER"
  export RANLIB="llvm-ranlib-$LLVMVER"
else
  export AR="llvm-ar"
  export RANLIB="llvm-ranlib"
fi

cmake ..                                               \
    -DCMAKE_INSTALL_PREFIX=/usr                        \
    -DUSE_NATIVE_INSTRUCTIONS=OFF                      \
    -DUSE_PRECOMPILED_HEADERS=OFF                      \
    -DCMAKE_C_FLAGS="$CFLAGS"                          \
    -DCMAKE_CXX_FLAGS="$CFLAGS"                        \
    -DCMAKE_EXE_LINKER_FLAGS="${LINKER_FLAG}"          \
    -DCMAKE_MODULE_LINKER_FLAGS="${LINKER_FLAG}"       \
    -DCMAKE_SHARED_LINKER_FLAGS="${LINKER_FLAG}"       \
    -DCMAKE_AR="$AR"                                   \
    -DCMAKE_RANLIB="$RANLIB"                           \
    -DUSE_SYSTEM_CURL=ON                               \
    -DUSE_FAUDIO=OFF                                   \
    -DUSE_SDL=ON                                       \
    -DUSE_SYSTEM_SDL=OFF                               \
    -DUSE_SYSTEM_FFMPEG=ON                             \
    -DUSE_SYSTEM_OPENCV=ON                             \
    -DUSE_SYSTEM_OPENAL=OFF                            \
    -DUSE_DISCORD_RPC=ON                               \
    -DOpenGL_GL_PREFERENCE=LEGACY                      \
    -DWITH_LLVM=ON                                     \
    -DLLVM_DIR="C:/Program Files/LLVM/lib/cmake/llvm"  \
    -DSTATIC_LINK_LLVM=ON                              \
    -DBUILD_RPCS3_TESTS=OFF                            \
    -DRUN_RPCS3_TESTS=OFF                              \
    -G Ninja

ninja; build_status=$?;

cd ..

# If it compiled succesfully let's deploy.
if [ "$build_status" -eq 0 ]; then
    .ci/deploy-windows-clang-cl.sh "x86_64"
fi
