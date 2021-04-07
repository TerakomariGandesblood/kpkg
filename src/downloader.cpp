#include "downloader.h"

#include <iostream>

namespace kpkg {

Downloader::Downloader(bool use_proxy) {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  http_handle_ = curl_easy_init();

  curl_easy_setopt(http_handle_, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(http_handle_, CURLOPT_WRITEFUNCTION, Downloader::write_data);
  curl_easy_setopt(
      http_handle_, CURLOPT_USERAGENT,
      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like "
      "Gecko) Chrome/91.0.4437.0 Safari/537.36 Edg/91.0.831.1");
  if (use_proxy) {
    curl_easy_setopt(http_handle_, CURLOPT_PROXY, "socks5://127.0.0.1:1080");
  }

  multi_handle_ = curl_multi_init();
  curl_multi_add_handle(multi_handle_, http_handle_);
}

bool Downloader::download(const std::string &url,
                          const std::string &file_name) {
  std::clog << "download file from: " << url << "\n";
  std::clog << "to: " << file_name << "\n";

  auto file{std::fopen(file_name.c_str(), "wb")};
  if (file) {
    curl_easy_setopt(http_handle_, CURLOPT_WRITEDATA, file);
  }

  curl_easy_setopt(http_handle_, CURLOPT_URL, url.c_str());

  std::int32_t still_running{};
  std::int32_t repeats{};

  curl_multi_perform(multi_handle_, &still_running);
  while (still_running != 0) {
    std::int32_t numfds{};

    if (auto mc{curl_multi_wait(multi_handle_, nullptr, 0, 1000, &numfds)};
        mc != CURLM_OK) {
      fprintf(stderr, "curl_multi_wait() failed, code %d.\n", mc);
      break;
    }

    if (numfds == 0) {
      repeats++;
      if (repeats > 1) {
        wait(100);
      }
    } else {
      repeats = 0;
    }

    curl_multi_perform(multi_handle_, &still_running);
  }

  fclose(file);

  return true;
}

Downloader::~Downloader() {
  curl_multi_remove_handle(multi_handle_, http_handle_);
  curl_easy_cleanup(http_handle_);
  curl_multi_cleanup(multi_handle_);
  curl_global_cleanup();
}

void Downloader::wait(std::int32_t ms) const {
  timeval wait{0, ms * 1000};
  select(0, nullptr, nullptr, nullptr, &wait);
}

std::size_t Downloader::write_data(void *ptr, std::size_t size,
                                   std::size_t nmemb, void *stream) {
  return std::fwrite(ptr, size, nmemb, static_cast<std::FILE *>(stream));
}

}  // namespace kpkg
