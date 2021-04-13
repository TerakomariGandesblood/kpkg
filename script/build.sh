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
    echo "Unknown option: $OPT"
    exit 1
    ;;
  esac
done

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  curl -L https://github.com/KaiserLancelot/kpkg/releases/download/v0.1.1/kpkg-0.1.1-ubuntu-20.04.deb \
    -o kpkg.deb

  sudo dpkg -i *.deb

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
    kpkg lcov fmt spdlog libarchive curl boost catch2 -i
  fi

  sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 400
  sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 400

  sudo ldconfig
else
  echo "The system does not support: $OSTYPE"
  exit 1
fi
