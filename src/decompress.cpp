#include "decompress.h"

#include <cstddef>
#include <cstdint>

#include <archive.h>
#include <archive_entry.h>

#include "error.h"

namespace {

void copy_data(struct archive *ar, struct archive *aw) {
  while (true) {
    const void *buff;
    std::size_t size;
    la_int64_t offset;

    la_ssize_t status = archive_read_data_block(ar, &buff, &size, &offset);
    if (status == ARCHIVE_EOF) {
      return;
    }
    if (status != ARCHIVE_OK) {
      kpkg::error(archive_error_string(aw));
    }

    status = archive_write_data_block(aw, buff, size, offset);
    if (status != ARCHIVE_OK) {
      kpkg::error(archive_error_string(aw));
    }
  }
}

}  // namespace

namespace kpkg {

std::string decompress(const std::string &file_name) {
  std::int32_t flags = (ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM |
                        ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS);

  auto archive = archive_read_new();
  archive_read_support_format_gnutar(archive);
  archive_read_support_filter_gzip(archive);

  auto extract = archive_write_disk_new();
  archive_write_disk_set_options(extract, flags);
  archive_write_disk_set_standard_lookup(extract);

  std::int32_t status =
      archive_read_open_filename(archive, file_name.c_str(), 10240);
  if (status != ARCHIVE_OK) {
    error("archive_read_open_filename error: {}", file_name);
  }

  std::string dir;
  while (true) {
    struct archive_entry *entry;
    status = archive_read_next_header(archive, &entry);
    if (status == ARCHIVE_EOF) {
      break;
    }
    if (status != ARCHIVE_OK) {
      error(archive_error_string(archive));
    }

    status = archive_write_header(extract, entry);
    if (status != ARCHIVE_OK) {
      error(archive_error_string(archive));
    }
    if (std::empty(dir)) {
      dir = archive_entry_pathname(entry);
    }

    if (archive_entry_size(entry) > 0) {
      copy_data(archive, extract);
    }

    status = archive_write_finish_entry(extract);
    if (status != ARCHIVE_OK) {
      error(archive_error_string(archive));
    }
  }

  archive_read_close(archive);
  archive_read_free(archive);
  archive_write_close(extract);
  archive_write_free(extract);

  if (dir.ends_with("/")) {
    return dir.substr(0, std::size(dir) - 1);
  }

  return dir;
}

}  // namespace kpkg
