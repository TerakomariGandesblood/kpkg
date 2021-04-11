#!/bin/bash

set -e

thread=false
memory=false

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

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  sudo dpkg -i temp/*.deb

  if [ ! -d "dependencies" ]; then
    echo "mkdir dependencies"
    mkdir dependencies
  fi

  cd dependencies

  if $thread; then
    kpkg lcov gflags fmt spdlog libarchive curl boost googletest -install -thread
  elif $memory; then
    kpkg lcov gflags fmt spdlog libarchive curl boost googletest -install -memory
  else
    kpkg lcov gflags fmt spdlog libarchive curl boost googletest -install
  fi

  sudo apt install valgrind clang-tidy-12

  sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 400
  sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 400
else
  echo "The system does not support: $OSTYPE"
  exit 1
fi
