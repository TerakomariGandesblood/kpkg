export CC=gcc-10 && export CXX=g++-10

# zlib
./configure --static
make -j$(nproc)
sudo make install

# libarchive(need zlib)
# https://github.com/libarchive/libarchive/wiki/BuildInstructions#gnu-autotools-builds
sh build/autogen.sh
./configure --disable-shared --disable-bsdtar --disable-bsdcat --disable-bsdcpio \
  --without-bz2lib --without-libb2 --without-lz4 --without-zstd --without-lzma \
  --without-cng --without-openssl --without-xml2 --without-expat
make -j$(nproc)
sudo make install

# zstd
# https://github.com/facebook/zstd#cmake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_TESTS=OFF \
  -DZSTD_BUILD_SHARED=OFF
cmake --build build --config Release -j$(nproc)
sudo cmake --build build --config Release --target install

# catch2
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#installing-catch2-from-git-repository
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DCATCH_BUILD_TESTING=OFF
cmake --build build --config Release -j$(nproc)
sudo cmake --build build --config Release --target install

# openssl
# https://github.com/openssl/openssl/blob/master/INSTALL.md#building-openssl
./Configure linux-x86_64 no-tests no-shared
make -j$(nproc)
sudo make install_sw

# curl(need openssl zlib)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_CURL_EXE=OFF -DBUILD_SHARED_LIBS=OFF \
  -DHTTP_ONLY=ON -DBUILD_TESTING=OFF
cmake --build build --config Release -j$(nproc)
sudo cmake --build build --config Release --target install

# rocksdb
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DWITH_GFLAGS=OFF -DUSE_RTTI=ON -DROCKSDB_BUILD_SHARED=OFF \
  -DWITH_TESTS=OFF -DWITH_BENCHMARK_TOOLS=OFF \
  -DWITH_CORE_TOOLS=OFF -DWITH_TOOLS=OFF \
  -DWITH_ALL_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build --config Release -j$(nproc)
sudo cmake --build build --config Release --target install

# fmt
# https://fmt.dev/latest/usage.html#building-the-library
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DFMT_TEST=OFF
cmake --build build --config Release -j$(nproc)
sudo cmake --build build --config Release --target install

# spdlog(need fmt)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DSPDLOG_BUILD_EXAMPLE=OFF -DSPDLOG_FMT_EXTERNAL=ON
cmake --build build --config Release -j$(nproc)
sudo cmake --build build --config Release --target install

# google benchmark
# https://github.com/google/benchmark#installation
# LLVMAR_EXECUTABLE, LLVMNM_EXECUTABLE and LLVMRANLIB_EXECUTABLE
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DBENCHMARK_ENABLE_TESTING=OFF
cmake --build build --config Release -j$(nproc)
sudo cmake --build build --config Release --target install

# sqlite(need zlib)
./configure --disable-shared --disable-tcl --disable-readline
make -j$(nproc)
sudo make install

# protobuf(need zlib)
./configure --disable-shared
make -j$(nproc)
sudo make install

# magic enum
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
  -DMAGIC_ENUM_OPT_BUILD_EXAMPLES=OFF -DMAGIC_ENUM_OPT_BUILD_TESTS=OFF
sudo cmake --build build --config Release --target install

# icu
# https://unicode-org.github.io/icu/userguide/icu4c/build.html
# FIXME
export CPPFLAGS="-DU_STATIC_IMPLEMENTATION"
./runConfigureICU Linux --disable-shared --enable-static \
  --disable-tests --disable-samples
make -j$(nproc)
sudo make install

# boost
./bootstrap.sh
sudo ./b2 toolset=gcc-10 install
