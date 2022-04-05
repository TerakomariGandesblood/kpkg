#include <qrencode.h>

int main() {
  auto qr_code = QRcode_encodeString("https://github.com/KaiserLancelot", 0,
                                     QRecLevel::QR_ECLEVEL_H,
                                     QRencodeMode::QR_MODE_8, true);
  QRcode_free(qr_code);
}
