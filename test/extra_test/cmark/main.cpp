#include <cstdlib>
#include <cstring>
#include <iostream>

#include <cmark.h>

int main() {
  auto str = "paragraph\n";
  auto html = cmark_markdown_to_html(str, std::strlen(str), CMARK_OPT_DEFAULT);
  std::cout << html << '\n';
  std::free(html);
}
