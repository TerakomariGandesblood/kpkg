#include "http.h"

#include <cstdint>

#include <klib/log.h>

namespace kpkg {

klib::Response http_get(const std::string &url, const std::string &proxy) {
  static klib::Request request;
  request.set_browser_user_agent();
  request.set_accept_encoding("gzip, deflate, br");
  request.set_doh_url("https://dns.google/dns-query");
  if (!std::empty(proxy)) {
    request.set_proxy(proxy);
  }
#ifndef NDEBUG
  request.verbose(true);
#endif

  auto response = request.get(url);
  auto status = response.status();
  if (status != klib::HttpStatus::HTTP_STATUS_OK) {
    klib::error("HTTP request failed, code: {}, reason: {}, url: {}",
                static_cast<std::int32_t>(status),
                klib::http_status_str(status), url);
  }

  return response;
}

}  // namespace kpkg
