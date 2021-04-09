#include <cassert>
#include <iostream>

#include <boost/json.hpp>

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
  assert(obj == jv);

  boost::json::error_code error;
  boost::json::monotonic_resource mr;
  boost::json::parse_options opt;
  opt.allow_comments = true;
  jv = boost::json::parse(boost::json::serialize(obj), error, &mr, opt);
  assert(!error && obj == jv);

  std::cout << obj << '\n';
}
