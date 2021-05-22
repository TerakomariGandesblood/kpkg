#include <pugixml.hpp>

#include "error.h"

// https://github.com/zeux/pugixml#example
int main() {
  pugi::xml_document doc;
  auto result = doc.load_file("data.xml");
  if (!result) {
    error(result.description());
  }

  auto children = doc.child("小纸条").children();
  auto iter = children.begin();

  EXPECT(iter->name() == std::string("收件人"));
  EXPECT(iter->text().as_string() == std::string("大元"));

  ++iter;

  EXPECT(iter->name() == std::string("发件人"));
  EXPECT(iter->text().as_string() == std::string("小张"));

  ++iter;
  EXPECT(iter == children.end());
}
