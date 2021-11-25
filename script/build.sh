#!/bin/bash

set -e

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  curl -L https://github.com/KaiserLancelot/klib/releases/download/v0.8.8/klib-0.8.8-Linux.deb \
    -o klib.deb
  sudo dpkg -i klib.deb

  # FIXME
  curl -L https://github.com/AppImage/AppImageKit/releases/download/12/appimagetool-x86_64.AppImage \
    -o appimagetool-x86_64.AppImage
  mkdir -p /home/.local/share/Nuitka/appimagetool-x86_64.AppImage/x86_64/12
  cp appimagetool-x86_64.AppImage /home/.local/share/Nuitka/appimagetool-x86_64.AppImage/x86_64/12/appimagetool-x86_64.AppImage
  sudo mkdir -p /root/.local/share/Nuitka/appimagetool-x86_64.AppImage/x86_64/12
  sudo mv appimagetool-x86_64.AppImage /root/.local/share/Nuitka/appimagetool-x86_64.AppImage/x86_64/12/appimagetool-x86_64.AppImage
else
  echo "The system does not support: $OSTYPE"
  exit 1
fi
