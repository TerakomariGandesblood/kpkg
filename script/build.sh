#!/bin/bash

set -e

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  curl -L https://github.com/KaiserLancelot/klib/releases/download/v1.11.0/klib-1.11.0-Linux.deb \
    -o klib.deb
  sudo dpkg -i klib.deb
else
  echo "The system does not support: $OSTYPE"
  exit 1
fi
