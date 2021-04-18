#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "error.h"

// https://www.boost.org/doc/libs/1_76_0/doc/html/string_algo.html
int main() {
  std::string str = " HELLO WORLD! ";

  boost::to_lower(str);
  EXPECT(str == " hello world! ");

  boost::trim_left(str);
  EXPECT(str == "hello world! ");

  boost::trim(str);
  EXPECT(str == "hello world!");

  str = "012345678";
  boost::trim_left_if(str, boost::is_any_of("02"));
  EXPECT(str == "12345678");

  str = "hello world!";
  // 有前缀 'i' 的版本, 大小写不敏感
  str = boost::to_lower_copy(boost::ireplace_all_copy(str, "hello", "goodbye"));
  EXPECT(str == "goodbye world!");

  // 注意 is_lower 只有当全部字符都是小写字母时为真
  EXPECT(!boost::all(str, boost::is_lower()));

  EXPECT(boost::all(str, boost::is_from_range('a', 'z') || boost::is_punct() ||
                             boost::is_space()));

  auto result = boost::find_last(str, "world");
  boost::to_upper(result);
  EXPECT(str == "goodbye WORLD!");

  boost::replace_first(str, "goodbye", "hello");
  EXPECT(str == "hello WORLD!");

  boost::erase_all(str, "ll");
  EXPECT(str == "heo WORLD!");

  boost::erase_head(str, 3);
  EXPECT(str == " WORLD!");

  str = boost::copy_range<std::string>(boost::find_tail(str, 2));
  EXPECT(str == "D!");

  str = "abc-*-ABC-*-aBc";
  auto iter = boost::make_find_iterator(
      str, boost::first_finder("abc", boost::is_iequal()));
  EXPECT(boost::copy_range<std::string>(*iter++) == "abc");
  EXPECT(boost::copy_range<std::string>(*iter++) == "ABC");
  EXPECT(boost::copy_range<std::string>(*iter++) == "aBc");

  str = "hello abc-*-ABC-*-aBc goodbye";
  std::vector<std::string> split_result;
  // 当输入包含连续分割符时, 默认情况下将其解释为其间有连续的字符串
  // 指定 boost::token_compress_on 可以去除这种空字符串
  boost::split(split_result, str, boost::is_any_of("-*"),
               boost::token_compress_on);
  EXPECT((split_result ==
          std::vector<std::string>{"hello abc", "ABC", "aBc goodbye"}));

  // 这里视为前后都有一个空字符串
  str = "|a|b|c|";
  boost::split(split_result, str, boost::is_any_of("|"),
               boost::token_compress_on);
  EXPECT((split_result == std::vector<std::string>{"", "a", "b", "c", ""}));

  str = boost::join(std::vector<std::string>{"a", "b", "c"}, "*");
  EXPECT(str == "a*b*c");
}
