#include <iostream>

#include <utf8cpp/utf8.h>

int main() {
  std::u16string str = u"你好世界";
  std::cout << utf8::utf16to8(str) << '\n';
}
