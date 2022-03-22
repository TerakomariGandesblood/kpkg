#include <klib/log.h>
#include <vips/vips8>

#ifdef KPKG_SANITIZER
#include <sanitizer/lsan_interface.h>
#endif

int main() {
#ifdef KPKG_SANITIZER
  __lsan_disable();
#endif

  if (vips_init("")) {
    klib::error("VIPS_INIT() failed");
  }

  auto image = vips::VImage::new_from_file("avatar.jpg");
  image.webpsave("avatar.wepb");
  image.jpegsave("avatar.jpeg");

#ifdef KPKG_SANITIZER
  __lsan_enable();
#endif
}
