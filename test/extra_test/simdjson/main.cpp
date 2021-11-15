#include <iostream>

#include <simdjson.h>

int main() {
  auto json = R"({"a":"aa","b":"bb"})"_padded;
  simdjson::ondemand::parser parser;
  auto doc = parser.iterate(json);
  std::cout << doc["a"] << '\n';
}
