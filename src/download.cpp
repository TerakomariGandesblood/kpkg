#include "download.h"

#include <sys/select.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <filesystem>

#include <curl/curl.h>

#include "error.h"

namespace {

std::size_t callback_func_std_string(void* contents, std::size_t size,
                                     std::size_t nmemb, std::string* s) {
  s->append(static_cast<char*>(contents), size * nmemb);
  return size * nmemb;
}

void wait(std::int32_t ms) {
  timeval wait{0, ms * 1000};
  select(0, nullptr, nullptr, nullptr, &wait);
}

std::size_t write_data(void* ptr, std::size_t size, std::size_t nmemb,
                       void* stream) {
  return std::fwrite(ptr, size, nmemb, static_cast<std::FILE*>(stream));
}

}  // namespace

namespace kpkg {

std::string get_page(const std::string& url, bool use_proxy) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  auto http_handle = curl_easy_init();
  if (!http_handle) {
    error("curl_easy_init() error");
  }

#ifndef NDEBUG
  curl_easy_setopt(http_handle, CURLOPT_VERBOSE, 1);
#endif

  curl_easy_setopt(http_handle, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(http_handle, CURLOPT_SSL_VERIFYPEER, 1);
  curl_easy_setopt(http_handle, CURLOPT_SSL_VERIFYHOST, 2);
  curl_easy_setopt(http_handle, CURLOPT_CAPATH, "/etc/ssl/certs");
  curl_easy_setopt(http_handle, CURLOPT_CAINFO,
                   "/etc/ssl/certs/ca-certificates.crt");
  curl_easy_setopt(
      http_handle, CURLOPT_USERAGENT,
      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) "
      "Chrome/91.0.4456.0 Safari/537.36 Edg/91.0.845.2");

  if (use_proxy) {
    curl_easy_setopt(http_handle, CURLOPT_PROXY, "socks5://127.0.0.1:1080");
  }

  std::string result;
  curl_easy_setopt(http_handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(http_handle, CURLOPT_WRITEDATA, &result);
  curl_easy_setopt(http_handle, CURLOPT_WRITEFUNCTION,
                   callback_func_std_string);

  if (curl_easy_perform(http_handle) != CURLE_OK) {
    error("curl_easy_perform() error");
  }

  curl_easy_cleanup(http_handle);
  curl_global_cleanup();

  if (std::empty(result)) {
    error("get page error");
  }

  return result;
}

void get_file(const std::string& url, const std::string& file_name,
              bool use_proxy) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  auto http_handle = curl_easy_init();
  if (!http_handle) {
    error("curl_easy_init() error");
  }

#ifndef NDEBUG
  curl_easy_setopt(http_handle, CURLOPT_VERBOSE, 1);
#endif

  curl_easy_setopt(http_handle, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt(http_handle, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(http_handle, CURLOPT_SSL_VERIFYPEER, 1);
  curl_easy_setopt(http_handle, CURLOPT_SSL_VERIFYHOST, 2);
  curl_easy_setopt(http_handle, CURLOPT_CAPATH, "/etc/ssl/certs");
  curl_easy_setopt(http_handle, CURLOPT_CAINFO,
                   "/etc/ssl/certs/ca-certificates.crt");
  curl_easy_setopt(
      http_handle, CURLOPT_USERAGENT,
      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) "
      "Chrome/91.0.4456.0 Safari/537.36 Edg/91.0.845.2");

  if (use_proxy) {
    curl_easy_setopt(http_handle, CURLOPT_PROXY, "socks5://127.0.0.1:1080");
  }

  auto file = std::fopen(file_name.c_str(), "wb");
  if (!file) {
    error("open file error");
  }

  curl_easy_setopt(http_handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(http_handle, CURLOPT_WRITEDATA, file);
  curl_easy_setopt(http_handle, CURLOPT_WRITEFUNCTION, write_data);

  auto multi_handle = curl_multi_init();
  curl_multi_add_handle(multi_handle, http_handle);

  std::int32_t still_running = 0;
  std::int32_t repeats = 0;

  if (curl_multi_perform(multi_handle, &still_running) != CURLM_OK) {
    error("curl_multi_perform() error");
  }
  while (still_running != 0) {
    std::int32_t numfds{};

    if (auto mc{curl_multi_wait(multi_handle, nullptr, 0, 1000, &numfds)};
        mc != CURLM_OK) {
      error("curl_multi_wait() failed");
    }

    if (numfds == 0) {
      ++repeats;
      if (repeats > 1) {
        wait(100);
      }
    } else {
      repeats = 0;
    }

    if (curl_multi_perform(multi_handle, &still_running) != CURLM_OK) {
      error("curl_multi_perform() error");
    }
  }

  fclose(file);

  curl_multi_remove_handle(multi_handle, http_handle);
  curl_easy_cleanup(http_handle);
  curl_multi_cleanup(multi_handle);
  curl_global_cleanup();

  if (std::filesystem::file_size(file_name) == 0) {
    error("get file error");
  }
}

}  // namespace kpkg
