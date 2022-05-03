#include <iostream>

#include <klib/log.h>
#include <klib/util.h>
#include <magic.h>
#include <scope_guard.hpp>

int main() {
  auto magic = magic_open(MAGIC_MIME_TYPE);
  SCOPE_EXIT { magic_close(magic); };
  if (!magic) {
    klib::error("Unable to initialize magic library");
  }

  if (magic_load(magic, nullptr) != 0) {
    klib::error("Cannot load magic database: {}", magic_error(magic));
  }

  const auto image = klib::read_file("test.jpg", true);

  std::cout << magic_buffer(magic, std::data(image), std::size(image)) << '\n';
}
