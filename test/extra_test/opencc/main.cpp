#include <iostream>
#include <string>

#include <opencc.h>

int main() {
  std::string str = "俱樂部 萬事俱備 一份子";

  const opencc::SimpleConverter converter("tw2sp.json");
  std::cout << converter.Convert(str) << '\n';
}
