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

## Third party

- klib ([MIT License](https://github.com/KaiserLancelot/klib/blob/main/LICENSE))
- mimalloc ([MIT License](https://github.com/microsoft/mimalloc/blob/master/LICENSE))
- RE2 ([BSD 3-Clause "New" or "Revised" License](https://github.com/google/re2/blob/main/LICENSE))
- Boost ([Boost License](https://www.boost.org/users/license.html))
- GSL ([MIT License](https://github.com/Microsoft/GSL/blob/master/LICENSE))
- CLI11 ([License](https://github.com/CLIUtils/CLI11/blob/main/LICENSE))
- fmt ([License](https://github.com/fmtlib/fmt/blob/master/LICENSE.rst))
- simdjson ([Apache License 2.0](https://github.com/simdjson/simdjson/blob/master/LICENSE))
- semver ([MIT License](https://github.com/Neargye/semver/blob/master/LICENSE))

## Build environment

- [kenv](https://github.com/KaiserLancelot/kenv)

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
