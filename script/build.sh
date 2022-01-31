#!/bin/bash

set -e

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  curl -L https://github.com/KaiserLancelot/klib/releases/download/v0.11.8/klib-0.11.8-Linux.deb \
    -o klib.deb
  sudo dpkg -i klib.deb

  # FIXME
  sudo apt update && sudo apt install bc
else
  echo "The system does not support: $OSTYPE"
  exit 1
fi
