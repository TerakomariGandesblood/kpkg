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
    kpkg fmt spdlog libarchive curl boost catch2 -i -t
  elif $memory; then
    kpkg fmt spdlog libarchive curl boost catch2 -i -m
  else
    kpkg fmt spdlog libarchive curl boost catch2 -i
  fi

  sudo apt install valgrind clang-tidy-12

  sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 400
  sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 400
else
  echo "The system does not support: $OSTYPE"
  exit 1
fi
