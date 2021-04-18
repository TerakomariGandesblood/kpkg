#include <boost/json/error.hpp>
#include <boost/json/monotonic_resource.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/parse_options.hpp>
#include <boost/json/serialize.hpp>
#include <boost/json/value.hpp>

#include "error.h"

// https://www.boost.org/doc/libs/1_75_0/libs/json/doc/html/index.html
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
}
