#include "http.h"

#include <klib/error.h>

namespace kpkg {

klib::Response http_get(const std::string &url, const std::string &proxy) {
  static klib::Request request;
  request.use_cookies(false);
  request.set_browser_user_agent();
  if (!std::empty(proxy)) {
    request.set_proxy(proxy);
  }
#ifndef NDEBUG
  request.verbose(true);
#endif

  auto response = request.get(url);
  if (!response.ok()) {
    klib::error(KLIB_CURR_LOC, "Status code is not ok: {}, url: {}",
                response.status_code(), url);
  }

  return response;
}

}  // namespace kpkg
