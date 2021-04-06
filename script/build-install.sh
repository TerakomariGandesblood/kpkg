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
    echo "mkdir dependencies"
    mkdir dependencies
fi

cd dependencies

# lcov
if [ ! -f "lcov-1.15.zip" ]; then
    wget -q https://github.com/linux-test-project/lcov/archive/refs/tags/v1.15.zip -O lcov-1.15.zip
fi
unzip -q lcov-*.zip
rm lcov-*.zip
cd lcov-*
sudo make install
cd ..
echo "Build and install lcov completed"

C_FLAGS=""
CXX_FLAGS=""

if $thread || $memory; then
    # libc++
    if [ ! -f "llvm-project-llvmorg-12.0.0-rc3.zip" ]; then
        wget -q https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-12.0.0-rc3.zip \
            -O llvm-project-llvmorg-12.0.0-rc3.zip
    fi
    unzip -q llvm-project-llvmorg-*.zip
    rm llvm-project-llvmorg-*.zip
    cd llvm-project-llvmorg-*

    if $thread; then
        cmake -S llvm -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_LIBCXX=ON \
            -DLLVM_ENABLE_PROJECTS="libcxx;libcxxabi" -DLLVM_USE_SANITIZER=Thread
        C_FLAGS="-g -fsanitize=thread"
        CXX_FLAGS="-g -fsanitize=thread -stdlib=libc++"
    fi

    if $memory; then
        cmake -S llvm -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_LIBCXX=ON \
            -DLLVM_ENABLE_PROJECTS="libcxx;libcxxabi" -DLLVM_USE_SANITIZER=MemoryWithOrigins
        C_FLAGS="-g -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls"
        CXX_FLAGS="-g -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls -stdlib=libc++"
    fi

    cmake --build build --config RelWithDebInfo -j$(nproc) --target cxx cxxabi
    sudo cmake --build build --config RelWithDebInfo --target install-cxx install-cxxabi
    cd ..
    echo "Build and install libc++ completed"
fi

# Catch2
if [ ! -f "Catch2-2.13.4.zip" ]; then
    wget -q https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.4.zip \
        -O Catch2-2.13.4.zip
fi
unzip -q Catch2-*.zip
rm Catch2-*.zip
cd Catch2-*
cmake -S . -B build -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCATCH_BUILD_TESTING=OFF -DBUILD_SHARED_LIBS=TRUE \
    -DCMAKE_C_FLAGS="$C_FLAGS" -DCMAKE_CXX_FLAGS="$CXX_FLAGS"
cmake --build build --config $BUILD_TYPE -j$(nproc)
sudo cmake --build build --config $BUILD_TYPE --target install
cd ..
echo "Build and install Catch2 completed"

# boost
if [ ! -f "boost_1_75_0.tar.gz" ]; then
    wget -q https://dl.bintray.com/boostorg/release/1.75.0/source/boost_1_75_0.tar.gz \
        -O boost_1_75_0.tar.gz
fi
tar -xf boost_*.tar.gz
rm boost_*.tar.gz
cd boost_*
./bootstrap.sh
if $thread || $memory; then
    ./b2 toolset=$toolset cflags="$C_FLAGS" cxxflags="$CXX_FLAGS"
    sudo ./b2 toolset=$toolset cflags="$C_FLAGS" cxxflags="$CXX_FLAGS" install
else
    ./b2 toolset=$toolset
    sudo ./b2 toolset=$toolset install
fi
cd ..
echo "Build and install boost completed"

# NOTE Add new dependency here

cd ..

sudo ldconfig

echo "Build and install completed"
