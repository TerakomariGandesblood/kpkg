#include <cstddef>
#include <string>
#include <vector>

#include <boost/json.hpp>

#include "error.h"
#include "library.h"
#include "port.h"

int main() {
  std::string s(port, static_cast<std::size_t>(port_size));

  boost::json::error_code error;
  boost::json::parse_options opt;
  opt.allow_comments = true;
  auto jv = boost::json::parse(s.data(), error, {}, opt);
  if (error) {
    kpkg::error("json parse error");
  }

  jv = jv.as_object();
  kpkg::export_gcc = jv.at("export_gcc").as_string().c_str();
  kpkg::export_clang = jv.at("export_clang").as_string().c_str();
  kpkg::export_memory_flag = jv.at("export_memory_flag").as_string().c_str();
  kpkg::export_thread_flag = jv.at("export_thread_flag").as_string().c_str();

  auto library = jv.at("port").as_array();
  std::vector<kpkg::Library> v;

  for (const auto& item : library) {
    v.push_back(boost::json::value_to<kpkg::Library>(item));
  }

  for (auto& item : v) {
    item.init();
    item.download();
    item.build(kpkg::Sanitize::Memory);
  }
}
