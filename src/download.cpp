#include "download.h"

#include <sys/select.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <curl/curl.h>

#include "error.h"

namespace kpkg {

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

std::string get_page(const std::string& url) {
  std::string result;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  auto curl = curl_easy_init();
  if (!curl) {
    kpkg::error("curl_easy_init() error");
  }

  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);
  curl_easy_setopt(curl, CURLOPT_PROXY, "socks5://127.0.0.1:1080");
  curl_easy_setopt(
      curl, CURLOPT_USERAGENT,
      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like "
      "Gecko) Chrome/91.0.4437.0 Safari/537.36 Edg/91.0.831.1");

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_func_std_string);

  if (curl_easy_perform(curl) != CURLE_OK) {
    kpkg::error("curl_easy_perform() error");
  }

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return result;
}

void get_file(const std::string& url, const std::string& file_name) {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  auto http_handle = curl_easy_init();

  curl_easy_setopt(http_handle, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(http_handle, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(http_handle, CURLOPT_PROXY, "socks5://127.0.0.1:1080");
  curl_easy_setopt(
      http_handle, CURLOPT_USERAGENT,
      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like "
      "Gecko) Chrome/91.0.4437.0 Safari/537.36 Edg/91.0.831.1");

  auto multi_handle = curl_multi_init();
  curl_multi_add_handle(multi_handle, http_handle);

  auto file{std::fopen(file_name.c_str(), "wb")};
  if (file) {
    curl_easy_setopt(http_handle, CURLOPT_WRITEDATA, file);
  }

  curl_easy_setopt(http_handle, CURLOPT_URL, url.c_str());

  std::int32_t still_running{};
  std::int32_t repeats{};

  curl_multi_perform(multi_handle, &still_running);
  while (still_running != 0) {
    std::int32_t numfds{};

    if (auto mc{curl_multi_wait(multi_handle, nullptr, 0, 1000, &numfds)};
        mc != CURLM_OK) {
      kpkg::error("curl_multi_wait() failed");
    }

    if (numfds == 0) {
      repeats++;
      if (repeats > 1) {
        wait(100);
      }
    } else {
      repeats = 0;
    }

    curl_multi_perform(multi_handle, &still_running);
  }

  fclose(file);

  curl_multi_remove_handle(multi_handle, http_handle);
  curl_easy_cleanup(http_handle);
  curl_multi_cleanup(multi_handle);
  curl_global_cleanup();
}

}  // namespace kpkg
