#!/bin/bash

set -e

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  #  curl -L https://github.com/KaiserLancelot/kpkg/releases/download/v0.1.4/kpkg-0.1.4-ubuntu-20.04.deb \
  #    -o kpkg.deb

  sudo dpkg -i temp/kpkg-0.2.0-Linux.deb

  sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
  sudo apt install -y clang-tidy-12 valgrind

  sudo update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-12 400
  sudo update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-12 400
  sudo update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-12 400
  sudo update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-12 400

  if [ ! -d "dependencies" ]; then
    echo "mkdir dependencies"
    mkdir dependencies
  fi

  cd dependencies

  kpkg install lcov fmt spdlog libarchive curl boost catch2 -i

  sudo ldconfig
else
  echo "The system does not support: $OSTYPE"
  exit 1
fi
