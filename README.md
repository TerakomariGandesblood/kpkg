# kpkg

[![Build](https://github.com/KaiserLancelot/kpkg/actions/workflows/build.yml/badge.svg)](https://github.com/KaiserLancelot/kpkg/actions/workflows/build.yml)
[![Platform](https://img.shields.io/badge/Platform-Debian%2011-brightgreen)](https://www.debian.org/)
[![Coverage Status](https://coveralls.io/repos/github/KaiserLancelot/kpkg/badge.svg?branch=main)](https://coveralls.io/github/KaiserLancelot/kpkg?branch=main)
[![GitHub Releases](https://img.shields.io/github/release/KaiserLancelot/kpkg)](https://github.com/KaiserLancelot/kpkg/releases/latest)
[![GitHub Downloads](https://img.shields.io/github/downloads/KaiserLancelot/kpkg/total)](https://github.com/KaiserLancelot/kpkg/releases)
[![GitHub License](https://img.shields.io/github/license/KaiserLancelot/kpkg)](https://github.com/KaiserLancelot/kpkg/blob/main/LICENSE)
[![996.icu](https://img.shields.io/badge/link-996.icu-red.svg)](https://996.icu)

---

C++ Library Manager

## Environment

- [kenv](https://github.com/KaiserLancelot/kenv)

## Third party

- klib ([MIT License](https://github.com/KaiserLancelot/klib/blob/main/LICENSE))
- mimalloc ([MIT License](https://github.com/microsoft/mimalloc/blob/master/LICENSE))
- Boost ([Boost License](https://www.boost.org/users/license.html))
- GSL ([MIT License](https://github.com/Microsoft/GSL/blob/master/LICENSE))
- CLI11 ([License](https://github.com/CLIUtils/CLI11/blob/main/LICENSE))
- fmt ([License](https://github.com/fmtlib/fmt/blob/master/LICENSE.rst))
- spdlog ([MIT License](https://github.com/gabime/spdlog/blob/v1.x/LICENSE))
- simdjson ([Apache License 2.0](https://github.com/simdjson/simdjson/blob/master/LICENSE))
- semver ([MIT License](https://github.com/Neargye/semver/blob/master/LICENSE))
- Catch2 ([Boost License](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))

## Build

```bash
cmake -S . -B build
cmake --build build --config Release -j"$(nproc)"
```

## Install

```bash
sudo cmake --build build --config Release --target install
```

## Uninstall

```bash
sudo cmake --build build --config Release --target uninstall
```

## Usage

### List installable libraries

```bash
kpkg list
```

### Install libraries

```bash
kpkg install boost
```

---

Thanks to [JetBrains](https://www.jetbrains.com/) for donating product licenses to help develop this project <a href="https://www.jetbrains.com/"><img src="logo/jetbrains.svg" width="94" align="center" /></a>
