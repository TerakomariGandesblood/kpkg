#include "program.h"

#include <algorithm>
#include <cstdlib>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gflags/gflags.h>
#include <boost/algorithm/string.hpp>

#include "error.h"
#include "version.h"

extern char port[];
extern int port_size;

DEFINE_bool(memory, false, "Build with MemorySanitizer");
DEFINE_bool(thread, false, "Build with ThreadSanitizer");
DEFINE_bool(proxy, false, "Use proxy");
DEFINE_bool(install, false, "Install package");

namespace kpkg {

Program::Program(std::int32_t argc, char** argv) {
  auto input_library = parse_program_options(argc, argv);

  std::tie(libraries_, package_to_be_install_) = read_from_port();

  for (const auto& item : input_library) {
    auto library = get_from_name(libraries_, item);
    auto dep = library.get_dependency();
    for (const auto& i : dep) {
      dependency_.push_back(get_from_name(libraries_, i));
    }
    library_to_be_built_.push_back(
        get_from_name(libraries_, library.get_name()));
  }

  if (sanitize_ != Sanitize::None) {
    dependency_.insert(std::begin(dependency_),
                       get_from_name(libraries_, "libc++"));
  }

  std::sort(std::begin(libraries_), std::end(libraries_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });

  std::sort(std::begin(dependency_), std::end(dependency_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });
  dependency_.erase(std::unique(std::begin(dependency_), std::end(dependency_),
                                [](const Library& lhs, const Library& rhs) {
                                  return lhs.get_name() == rhs.get_name();
                                }),
                    std::end(dependency_));

  std::sort(std::begin(library_to_be_built_), std::end(library_to_be_built_),
            [](const Library& lhs, const Library& rhs) {
              return lhs.get_name() < rhs.get_name();
            });
  library_to_be_built_.erase(
      std::unique(std::begin(library_to_be_built_),
                  std::end(library_to_be_built_),
                  [](const Library& lhs, const Library& rhs) {
                    return lhs.get_name() == rhs.get_name();
                  }),
      std::end(library_to_be_built_));

  for (auto iter = std::begin(library_to_be_built_);
       iter != std::end(library_to_be_built_);) {
    if (std::find_if(std::begin(dependency_), std::end(dependency_),
                     [iter](const Library& item) {
                       return item.get_name() == iter->get_name();
                     }) != std::end(dependency_)) {
      iter = library_to_be_built_.erase(iter);
    } else {
      ++iter;
    }
  }

  for (auto iter = std::begin(dependency_); iter != std::end(dependency_);
       ++iter) {
    if (iter->get_name() == "libc++") {
      std::swap(*iter, dependency_.front());
      break;
    }
  }
}

void Program::print_dependency() const {
  if (std::empty(dependency_)) {
    return;
  }

  fmt::print("The following dependent libraries will be installed: ");

  for (const auto& item : dependency_) {
    fmt::print("{} ", item.get_name());
  }

  fmt::print("\n");
}

void Program::print_library_to_be_built() const {
  if (std::empty(library_to_be_built_)) {
    error("library_to_be_built_ is empty");
  }

  fmt::print("The following libraries will be installed: ");

  for (const auto& item : library_to_be_built_) {
    fmt::print("{} ", item.get_name());
  }

  fmt::print("\n");
}

std::vector<std::string> Program::parse_program_options(std::int32_t argc,
                                                        char* argv[]) {
  gflags::SetUsageMessage(fmt::format(R"(

Usage: {} [options] library ...

Allowed options:

    -version             print version string
    -help                produce help message
    -memory              Build with MemorySanitizer
    -thread              Build with ThreadSanitizer
    -proxy               Use proxy
    -install             Install package)",
                                      argv[0]));
  gflags::SetVersionString(
      fmt::format("{}.{}.{}", KPKG_VER_MAJOR, KPKG_VER_MINOR, KPKG_VER_PATCH));
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_proxy) {
    use_proxy_ = true;
  }
  if (FLAGS_install) {
    install_ = true;
  }

  if (FLAGS_memory) {
    sanitize_ = Sanitize::Memory;
  } else if (FLAGS_thread) {
    sanitize_ = Sanitize::Thread;
  }

  if (argc == 1) {
    kpkg::error("need a library name");
  }

  std::vector<std::string> result;
  for (std::int32_t i = 1; i < argc; ++i) {
    result.emplace_back(argv[i]);
  }

  return result;
}

std::pair<std::vector<Library>, std::vector<std::string>>
Program::read_from_port() {
  std::string_view s(R"({
  "install": [
    "sudo apt update",
    "sudo apt install -y gcc-10 g++-10",
    "sudo bash -c \"$(wget -O - https://apt.llvm.org/llvm.sh)\"",
    "sudo apt install -y libc++-12-dev libc++abi-12-dev",
    "sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-12 400",
    "sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-12 400",
    "sudo update-alternatives --install /usr/bin/llvm-ar llvm-ar /usr/bin/llvm-ar-12 400",
    "sudo update-alternatives --install /usr/bin/llvm-nm llvm-nm /usr/bin/llvm-nm-12 400",
    "sudo update-alternatives --install /usr/bin/llvm-ranlib llvm-ranlib /usr/bin/llvm-ranlib-12 400",
    "sudo update-alternatives --install /usr/bin/lld lld /usr/bin/lld-12 400",
    "sudo update-alternatives --install /usr/bin/ld.lld ld.lld /usr/bin/ld.lld-12 400",
    "sudo apt install -y re2c autoconf automake libtool m4 tcl",
    "sudo apt install valgrind clang-tidy-12",
    "sudo update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-12 400",
    "sudo update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-12 400",
    "sudo update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-12 400",
    "sudo update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-12 400"
  ],
  "port": [
    {
      "name": "CMake",
      "releases_url": "",
      "tags_url": "https://api.github.com/repos/Kitware/CMake/tags",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "./bootstrap --parallel=\"$(nproc)\" -- -DCMAKE_BUILD_TYPE:STRING=Release",
        "make -j\"$(nproc)\"",
        "sudo make install"
      ]
    },
    {
      "name": "ninja",
      "releases_url": "https://api.github.com/repos/ninja-build/ninja/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "doxygen",
      "releases_url": "",
      "tags_url": "https://api.github.com/repos/doxygen/doxygen/tags",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "lcov",
      "releases_url": "https://api.github.com/repos/linux-test-project/lcov/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": ["sudo make install"]
    },
    {
      "name": "libc++",
      "releases_url": "",
      "tags_url": "",
      "tag_name": "llvmorg-12.0.0-rc5",
      "download_url": "https://api.github.com/repos/llvm/llvm-project/tarball/refs/tags/llvmorg-12.0.0-rc5",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_LIBCXX=ON -DLLVM_ENABLE_PROJECTS=\"libcxx;libcxxabi\"",
        "cmake --build build --config Release -j\"$(nproc)\" --target cxx cxxabi",
        "sudo cmake --build build --config Release --target install-cxx install-cxxabi",
        "cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_USE_SANITIZER=MemoryWithOrigins -DLLVM_ENABLE_LIBCXX=ON -DLLVM_ENABLE_PROJECTS=\"libcxx;libcxxabi\"",
        "cmake -S llvm -B build -DCMAKE_BUILD_TYPE=Release -DLLVM_USE_SANITIZER=Thread -DLLVM_ENABLE_LIBCXX=ON -DLLVM_ENABLE_PROJECTS=\"libcxx;libcxxabi\""
      ]
    },
    {
      "name": "zlib",
      "releases_url": "",
      "tags_url": "https://api.github.com/repos/madler/zlib/tags",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "./configure --static",
        "make -j\"$(nproc)\"",
        "sudo make install"
      ]
    },
    {
      "name": "libarchive",
      "releases_url": "https://api.github.com/repos/libarchive/libarchive/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": ["zlib"],
      "cwd": "",
      "cmd": [
        "sh build/autogen.sh",
        "./configure --disable-shared --disable-bsdtar --disable-bsdcat --disable-bsdcpio --without-bz2lib --without-libb2 --without-lz4 --without-zstd --without-lzma --without-cng --without-openssl --without-xml2 --without-expat",
        "make -j\"$(nproc)\"",
        "sudo make install"
      ]
    },
    {
      "name": "zstd",
      "releases_url": "https://api.github.com/repos/facebook/zstd/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "build/cmake",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_TESTS=OFF -DZSTD_BUILD_SHARED=OFF",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "openssl",
      "releases_url": "",
      "tags_url": "",
      "tag_name": "OpenSSL_1_1_1k",
      "download_url": "https://api.github.com/repos/openssl/openssl/tarball/refs/tags/OpenSSL_1_1_1k",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "./Configure linux-x86_64 no-tests no-shared no-asm",
        "make -j\"$(nproc)\"",
        "sudo make install_sw"
      ]
    },
    {
      "name": "curl",
      "releases_url": "https://api.github.com/repos/curl/curl/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": ["zlib", "openssl"],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_CURL_EXE=OFF -DBUILD_SHARED_LIBS=OFF -DHTTP_ONLY=ON -DBUILD_TESTING=OFF",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "rocksdb",
      "releases_url": "",
      "tags_url": "https://api.github.com/repos/facebook/rocksdb/tags",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DWITH_GFLAGS=OFF -DUSE_RTTI=ON -DROCKSDB_BUILD_SHARED=OFF -DWITH_TESTS=OFF -DWITH_BENCHMARK_TOOLS=OFF -DWITH_CORE_TOOLS=OFF -DWITH_TOOLS=OFF -DWITH_ALL_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "fmt",
      "releases_url": "https://api.github.com/repos/fmtlib/fmt/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DFMT_TEST=OFF",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "spdlog",
      "releases_url": "https://api.github.com/repos/gabime/spdlog/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": ["fmt"],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DSPDLOG_BUILD_EXAMPLE=OFF -DSPDLOG_FMT_EXTERNAL=ON",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "benchmark",
      "releases_url": "https://api.github.com/repos/google/benchmark/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_TESTING=OFF",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "sqlite",
      "releases_url": "",
      "tags_url": "https://api.github.com/repos/sqlite/sqlite/tags",
      "tag_name": "",
      "download_url": "",
      "dependency": ["zlib"],
      "cwd": "",
      "cmd": [
        "./configure --disable-shared --disable-tcl --disable-readline",
        "make -j\"$(nproc)\"",
        "sudo make install"
      ]
    },
    {
      "name": "protobuf",
      "releases_url": "https://api.github.com/repos/protocolbuffers/protobuf/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": ["zlib"],
      "cwd": "",
      "cmd": [
        "./autogen.sh",
        "./configure --disable-shared",
        "make -j\"$(nproc)\"",
        "sudo make install"
      ]
    },
    {
      "name": "magic_enum",
      "releases_url": "https://api.github.com/repos/Neargye/magic_enum/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DMAGIC_ENUM_OPT_BUILD_EXAMPLES=OFF -DMAGIC_ENUM_OPT_BUILD_TESTS=OFF",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "boost",
      "releases_url": "",
      "tags_url": "",
      "tag_name": "boost-1.75.0",
      "download_url": "https://dl.bintray.com/boostorg/release/1.75.0/source/boost_1_75_0.tar.gz",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "./bootstrap.sh",
        "sudo ./b2 --with-json cflags=\"-fPIC\" cxxflags=\"-fPIC\" toolset=gcc-10 link=static install",
        "sudo ./b2 --with-json cflags=\"-fPIC -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls\" cxxflags=\"-fPIC -fsanitize=memory -fsanitize-memory-track-origins -fsanitize-memory-use-after-dtor -fno-omit-frame-pointer -fno-optimize-sibling-calls -stdlib=libc++\" toolset=clang-12 link=static install",
        "sudo ./b2 --with-json cflags=\"-fPIC -fsanitize=thread\" cxxflags=\"-fPIC -fsanitize=thread -stdlib=libc++\" toolset=clang-12 link=static install"
      ]
    },
    {
      "name": "oneTBB",
      "releases_url": "https://api.github.com/repos/oneapi-src/oneTBB/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DTBB_TEST=OFF",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "icu",
      "releases_url": "https://api.github.com/repos/unicode-org/icu/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "icu4c/source",
      "cmd": [
        "./runConfigureICU Linux --disable-tests --disable-samples",
        "make -j\"$(nproc)\"",
        "sudo make install"
      ]
    },
    {
      "name": "googletest",
      "releases_url": "https://api.github.com/repos/google/googletest/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_GMOCK=OFF",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    },
    {
      "name": "gflags",
      "releases_url": "https://api.github.com/repos/gflags/gflags/releases/latest",
      "tags_url": "",
      "tag_name": "",
      "download_url": "",
      "dependency": [],
      "cwd": "",
      "cmd": [
        "export CFLAGS=\"$CFLAGS -DSTRIP_FLAG_HELP=1\" && export CXXFLAGS=\"$CXXFLAGS -DSTRIP_FLAG_HELP=1\" && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release",
        "cmake --build build --config Release -j\"$(nproc)\"",
        "sudo cmake --build build --config Release --target install"
      ]
    }
  ]
}
)");

  boost::json::error_code error_code;
  auto jv = boost::json::parse(s.data(), error_code, {});
  if (error_code) {
    error("json parse error");
  }

  jv = jv.as_object();

  std::vector<std::string> install;
  auto arr = jv.at("install").as_array();
  for (const auto& item : arr) {
    install.emplace_back(item.as_string().c_str());
  }

  auto ports = jv.at("port").as_array();
  std::vector<Library> ret;

  for (const auto& item : ports) {
    ret.push_back(boost::json::value_to<Library>(item));
  }

  return {ret, install};
}

Library Program::get_from_name(const std::vector<Library>& libraries,
                               const std::string& name) {
  for (const auto& item : libraries) {
    if (boost::to_lower_copy(item.get_name()) == boost::to_lower_copy(name)) {
      return item;
    }
  }
  error("can not find this library: {}", name);
}

}  // namespace kpkg
