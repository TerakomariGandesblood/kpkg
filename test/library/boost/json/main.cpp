#include <cstdint>
#include <string>

#include <boost/json.hpp>

#include "error.h"

class Foo {
 public:
  Foo(int32_t id, const std::string& name) : id_(id), name_(name) {}

  [[nodiscard]] std::int32_t get_id() const { return id_; }

  [[nodiscard]] const std::string& get_name() const { return name_; }

 private:
  std::int32_t id_;
  std::string name_;
};

Foo tag_invoke(boost::json::value_to_tag<Foo>, const boost::json::value& jv) {
  const auto& obj = jv.as_object();
  return Foo{value_to<std::int32_t>(obj.at("id")),
             value_to<std::string>(obj.at("name"))};
}

// https://www.boost.org/doc/libs/1_76_0/libs/json/doc/html/index.html
int main() {
  boost::json::object obj;
  obj["pi"] = 3.141;
  obj["happy"] = true;
  obj["name"] = "Boost";
  obj["nothing"] = nullptr;
  obj["answer"].emplace_object()["everything"] = 42;
  obj["list"] = {1, 0, 2};
  obj["object"] = {{"currency", "USD"}, {"value", 42.99}};

  boost::json::value jv = {{"pi", 3.141},
                           {"happy", true},
                           {"name", "Boost"},
                           {"nothing", nullptr},
                           {"answer", {{"everything", 42}}},
                           {"list", {1, 0, 2}},
                           {"object", {{"currency", "USD"}, {"value", 42.99}}}};
  EXPECT(obj == jv);

  boost::json::error_code error;
  boost::json::monotonic_resource mr;
  boost::json::parse_options opt;
  opt.allow_comments = true;
  jv = boost::json::parse(boost::json::serialize(obj), error, &mr, opt);

  EXPECT(!error && obj == jv);

  jv = boost::json::parse(R"({"id":123,"name":"kaiser"})", error, &mr, opt);
  auto foo = boost::json::value_to<Foo>(jv);

  EXPECT(foo.get_id() == 123);
  EXPECT(foo.get_name() == "kaiser");
}
