#include <klib/log.h>
#include <vips/vips8>

int main() {
  if (VIPS_INIT("klib")) {
    klib::error("VIPS_INIT() failed");
  }

  auto image = vips::VImage::new_from_file("avatar.jpg");
  image.webpsave("avatar.wepb");
  image.jpegsave("avatar.jpeg");
}
