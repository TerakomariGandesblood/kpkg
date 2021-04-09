# kpkg

[![Build](https://github.com/KaiserLancelot/kpkg/actions/workflows/build.yml/badge.svg)](https://github.com/KaiserLancelot/kpkg/actions/workflows/build.yml)
[![Coverage Status](https://coveralls.io/repos/github/KaiserLancelot/kpkg/badge.svg?branch=master)](https://coveralls.io/github/KaiserLancelot/kpkg?branch=master)
[![GitHub License](https://img.shields.io/github/license/KaiserLancelot/kpkg)](https://raw.githubusercontent.com/KaiserLancelot/kpkg/master/LICENSE)
[![996.icu](https://img.shields.io/badge/link-996.icu-red.svg)](https://996.icu)
[![GitHub Releases](https://img.shields.io/github/release/KaiserLancelot/kpkg)](https://github.com/KaiserLancelot/kpkg/releases/latest)
[![GitHub Downloads](https://img.shields.io/github/downloads/KaiserLancelot/kpkg/total)](https://github.com/KaiserLancelot/kpkg/releases)
[![Bugs](https://img.shields.io/github/issues/KaiserLancelot/kpkg/bug)](https://github.com/KaiserLancelot/kpkg/issues?q=is%3Aopen+is%3Aissue+label%3Abug)

---

C++ LibraryInfo Manager

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
