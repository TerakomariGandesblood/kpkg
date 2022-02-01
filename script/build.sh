#!/bin/bash

set -e

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  curl -L https://github.com/KaiserLancelot/klib/releases/download/v0.12.0/klib-0.12.0-Linux.deb \
    -o klib.deb
  sudo dpkg -i klib.deb

  # FIXME
  curl -L https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-x86_64.AppImage \
    -o appimagetool-x86_64.AppImage
  sudo mkdir -p /root/.local/share/Nuitka/appimagetool-x86_64.AppImage/x86_64/13
  sudo mv appimagetool-x86_64.AppImage /root/.local/share/Nuitka/appimagetool-x86_64.AppImage/x86_64/13/appimagetool-x86_64.AppImage

  # FIXME
  sudo cp $(dirname $0)/libunistring.pc /usr/local/lib/pkgconfig
else
  echo "The system does not support: $OSTYPE"
  exit 1
fi
