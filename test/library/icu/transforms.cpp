#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include <unicode/translit.h>
#include <unicode/uclean.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/utypes.h>

// https://unicode-org.github.io/icu/userguide/transforms/general/#using-transliterators
int main() {
  std::ifstream ifs("data.txt");
  if (!ifs) {
    std::cerr << "can not open this file\n";
    std::exit(EXIT_FAILURE);
  }

  std::string data;
  data.resize(static_cast<std::string::size_type>(
      ifs.seekg(0, std::ifstream::end).tellg()));
  ifs.seekg(0, std::ifstream::beg)
      .read(data.data(), static_cast<std::streamsize>(std::size(data)));

  icu::UnicodeString str(data.c_str());

  UErrorCode status = U_ZERO_ERROR;
  auto trans =
      icu::Transliterator::createInstance("Hans-Hant", UTRANS_FORWARD, status);
  if (U_FAILURE(status)) {
    std::cerr << "error: " << u_errorName(status) << '\n';
    std::exit(EXIT_FAILURE);
  }

  trans->transliterate(str);

  std::cout << str << '\n';

  delete trans;
  u_cleanup();
}
