#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include <archive.h>
#include <archive_entry.h>

namespace {

[[noreturn]] void error(const char *msg) {
  std::cerr << msg << '\n';
  std::exit(EXIT_FAILURE);
}

std::int32_t copy_data(struct archive *ar, struct archive *aw) {
  while (true) {
    const void *buff;
    std::size_t size;
    la_int64_t offset;

    la_ssize_t status = archive_read_data_block(ar, &buff, &size, &offset);
    if (status == ARCHIVE_EOF) {
      return ARCHIVE_OK;
    }
    if (status != ARCHIVE_OK) {
      error(archive_error_string(aw));
    }

    status = archive_write_data_block(aw, buff, size, offset);
    if (status != ARCHIVE_OK) {
      error(archive_error_string(aw));
    }
  }
}

}  // namespace

// https://github.com/libarchive/libarchive/blob/master/examples/minitar/minitar.c
void compress(const std::string &file_name) {}

// https://github.com/libarchive/libarchive/wiki/Examples
void decompress(const std::string &file_name) {
  std::int32_t flags = (ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM |
                        ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS);

  auto archive = archive_read_new();
  archive_read_support_format_gnutar(archive);
  archive_read_support_format_zip(archive);
  archive_read_support_filter_gzip(archive);

  auto extract = archive_write_disk_new();
  archive_write_disk_set_options(extract, flags);
  archive_write_disk_set_standard_lookup(extract);

  std::int32_t status =
      archive_read_open_filename(archive, file_name.c_str(), 10240);
  if (status != ARCHIVE_OK) {
    error("archive_read_open_filename error");
  }

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

    if (archive_entry_size(entry) > 0) {
      status = copy_data(archive, extract);
      if (status != ARCHIVE_OK) {
        error(archive_error_string(archive));
      }
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
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    error("need a file name");
  }

  std::string file_name = argv[1];

  if (!std::filesystem::exists(file_name)) {
    error("the file not exists");
  }

  if (file_name.ends_with("zip") || file_name.ends_with("tar.gz")) {
    decompress(file_name);
  } else {
    compress(file_name);
  }
}
