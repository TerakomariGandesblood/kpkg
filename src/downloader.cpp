#include "downloader.h"

#include <cstdint>
#include <filesystem>
#include <memory>

#include <klib/error.h>

namespace kpkg {

namespace {

std::int32_t download_event_callback(aria2::Session* session,
                                     aria2::DownloadEvent event,
                                     aria2::A2Gid gid, void* user_data) {
  (void)user_data;

  switch (event) {
    case aria2::EVENT_ON_DOWNLOAD_COMPLETE:
      break;
    case aria2::EVENT_ON_DOWNLOAD_ERROR:
      klib::error("download error");
    default:
      return 0;
  }

  std::unique_ptr<aria2::DownloadHandle, decltype(aria2::deleteDownloadHandle)*>
      dh(aria2::getDownloadHandle(session, gid), aria2::deleteDownloadHandle);
  if (!dh) {
    klib::error("aria2::getDownloadHandle failed");
  }

  if (dh->getNumFiles() > 0) {
    std::filesystem::path path(dh->getFile(1).path);
    if (!std::filesystem::exists(path)) {
      klib::error("no file: {}", path.string());
    }
  }

  return 0;
}

}  // namespace

HTTPDownloader::HTTPDownloader(const std::string& proxy) {
  auto rc = aria2::libraryInit();
  if (rc != 0) {
    klib::error("aria2::libraryInit failed");
  }

  if (!std::empty(proxy)) {
    options_.emplace_back("http-proxy", proxy);
    options_.emplace_back("https-proxy", proxy);
  }
  options_.emplace_back("auto-file-renaming", "false");
  options_.emplace_back("allow-overwrite", "true");
  options_.emplace_back("max-connection-per-server", "16");
}

HTTPDownloader::~HTTPDownloader() {
  auto rc = aria2::libraryDeinit();
  if (rc != 0) {
    klib::error("aria2::libraryDeinit failed");
  }
}

void HTTPDownloader::download(const std::string& url,
                              const std::string& file_name) {
  auto free_session = [](aria2::Session* ptr) {
    auto rc = aria2::sessionFinal(ptr);
    if (rc != 0) {
      klib::error("aria2::sessionFinal failed");
    }
  };

  aria2::SessionConfig config;
  config.downloadEventCallback = download_event_callback;
  std::unique_ptr<aria2::Session, decltype(free_session)> session(
      aria2::sessionNew(options_, config), free_session);
  if (!session) {
    klib::error("aria2::sessionNew failed");
  }

  aria2::KeyVals options;
  if (!std::empty(file_name)) {
    options.emplace_back("out", file_name);
  }

  aria2::A2Gid gid;
  auto rc = aria2::addUri(session.get(), &gid, {url}, options);
  if (rc != 0) {
    klib::error("aria2::addUri failed");
  }

  rc = aria2::run(session.get(), aria2::RUN_DEFAULT);
  if (rc != 0) {
    klib::error("aria2::run failed");
  }
}

}  // namespace kpkg
