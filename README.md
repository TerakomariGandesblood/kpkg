# kpkg

[![Platform](https://img.shields.io/badge/Platform-Debian%2011-brightgreen)](https://www.debian.org/)
[![Build](https://github.com/KaiserLancelot/kpkg/actions/workflows/build.yml/badge.svg)](https://github.com/KaiserLancelot/kpkg/actions/workflows/build.yml)
[![Coverage Status](https://coveralls.io/repos/github/KaiserLancelot/kpkg/badge.svg?branch=main)](https://coveralls.io/github/KaiserLancelot/kpkg?branch=main)
[![GitHub License](https://img.shields.io/github/license/KaiserLancelot/kpkg)](https://github.com/KaiserLancelot/kpkg/blob/main/LICENSE)
[![996.icu](https://img.shields.io/badge/link-996.icu-red.svg)](https://996.icu)
[![GitHub Releases](https://img.shields.io/github/release/KaiserLancelot/kpkg)](https://github.com/KaiserLancelot/kpkg/releases/latest)
[![GitHub Downloads](https://img.shields.io/github/downloads/KaiserLancelot/kpkg/total)](https://github.com/KaiserLancelot/kpkg/releases)

---

C++ Library Manager

## Environment

- Debian 11
- GCC 11 or Clang 13

## Dependency

- [klib](https://github.com/KaiserLancelot/klib)
- [Boost](https://www.boost.org/)
- [CLI11](https://github.com/CLIUtils/CLI11)
- [fmt](https://github.com/fmtlib/fmt)
- [spdlog](https://github.com/gabime/spdlog)
- [simdjson](https://github.com/simdjson/simdjson)
- [semver](https://github.com/Neargye/semver)
- [aria2](https://github.com/aria2/aria2)

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
sudo kpkg install boost
```

---

Thanks to [JetBrains](https://www.jetbrains.com/) for donating product licenses to help develop this project <a href="https://www.jetbrains.com/"><img src="logo/jetbrains.svg" width="94" align="center" /></a>
