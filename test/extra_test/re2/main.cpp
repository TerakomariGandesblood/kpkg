#include <iostream>
#include <string>

#include <re2/re2.h>

int main() {
  re2::RE2 regex(R"(([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?)");
  const std::string str = "https://www.abc.com";
  std::cout << std::boolalpha << re2::RE2::FullMatch(str, regex) << '\n';
}
