#!/bin/bash

set -e

source $(dirname "$0")/install-system.sh

thread=false
memory=false
BUILD_TYPE=Release
toolset=gcc-10

while getopts 'mt' OPT; do
    case $OPT in
    m)
        memory=true
        ;;
    t)
        thread=true
        ;;
    ?)
        exit 1
        ;;
    esac
done

if $thread && $memory; then
    echo "error"
    exit 1
fi

if $thread || $memory; then
    export CC=clang
    export CXX=clang++
    BUILD_TYPE=RelWithDebInfo
    toolset=clang-12
fi

if [ ! -d "dependencies" ]; then
    echo "The dependencies directory does not exist"
    exit 1
fi

cd dependencies

cd lcov-*
sudo make install
cd ..
echo "Install lcov completed"

if [ -d llvm-project-llvmorg-* ]; then
    cd llvm-project-llvmorg-*
    sudo cmake --build build --config RelWithDebInfo --target install-cxx install-cxxabi
    cd ..
    echo "Install libc++ completed"
fi

C_FLAGS=""
CXX_FLAGS=""

if $thread; then
    C_FLAGS="-g -fsanitize=thread"
    CXX_FLAGS="-g -fsanitize=thread -stdlib=libc++"
fi

if $memory; then
    C_FLAGS="-g -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls"
    CXX_FLAGS="-g -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls -stdlib=libc++"
fi

cd Catch2-*
sudo cmake --build build --config $BUILD_TYPE --target install
cd ..
echo "Install Catch2 completed"

cd boost_*
if $thread || $memory; then
    sudo ./b2 toolset=$toolset cflags="$C_FLAGS" cxxflags="$CXX_FLAGS" install
else
    sudo ./b2 toolset=$toolset install
fi
cd ..
echo "Install boost completed"

# NOTE Add new dependency here

cd ..

sudo ldconfig

echo "Install completed"
