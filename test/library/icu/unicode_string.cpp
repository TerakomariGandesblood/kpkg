#include <cstdint>
#include <iostream>

#include <unicode/schriter.h>
#include <unicode/unistr.h>

#include "error.h"

int main() {
  // 使用平台默认字符编码
  icu::UnicodeString str = "你好世界";
  EXPECT(str == "你好世界");

  str = "a string";
  // 注意, 下面这些都改变了 str
  str.toUpper();
  EXPECT(str == "A STRING");

  str = "  你好世界  ";
  str.trim();
  EXPECT(str == "你好世界");

  str = "𝄞𝄞";
  // code unit
  EXPECT(str.length() == 4);
  // code points(占据 1 个或 2 个 char16_t code units)
  // 没有标注 32 的成员函数都是指 code units
  EXPECT(str.countChar32() == 2);

  // 截断
  str.truncate(2);
  EXPECT(str.length() == 2);

  str = "你好";
  EXPECT(icu::UnicodeString(str.charAt(1)) == "好");
  str = "𝄞𝄞";
  EXPECT(icu::UnicodeString(str.char32At(1)) == "𝄞");

  // 相当与 clear()
  str.remove();

  str = "𝄞𝄞";
  str.remove(2);
  EXPECT(str == "𝄞");

  str = "𝄞你";
  EXPECT(str.indexOf("你") == 2);

  str = "你好好";
  str.findAndReplace("好", "不好");
  EXPECT(str == "你不好不好");

  // 只读视图
  str.tempSubString(1);

  str = "helLo wOrld!";
  str.toTitle(nullptr);
  EXPECT(str == "Hello World!");

  str = "𝄞𝄞你好";
  auto length = str.length();
  // 获取每一个字符
  for (std::int32_t idx = 0; idx < length; idx = str.moveIndex32(idx, 1)) {
    std::cout << str.char32At(idx) << '\t';
  }
  std::cout << '\n';

  // 同上
  icu::StringCharacterIterator iter(str);
  while (iter.hasNext()) {
    std::cout << iter.next32PostInc() << '\t';
  }
  std::cout << '\n';

  icu::UnicodeString s1 = "weißkopfseeadler";
  icu::UnicodeString s2 = "weisskopfseeadler";
  // 大小写折叠
  EXPECT(s1.foldCase() == s2.foldCase());
}
