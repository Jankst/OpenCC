rm -rf build
mkdir -p build
gyp --depth=. --generator-output=build nacl.gyp

make -C build \
CC="$NACL_SDK_ROOT/toolchain/linux_x86_newlib/bin/i686-nacl-gcc" \
CXX="$NACL_SDK_ROOT/toolchain/linux_x86_newlib/bin/i686-nacl-g++"
cp build/out/Default/*.nexe .
