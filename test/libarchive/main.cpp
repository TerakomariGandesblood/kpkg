#include <fcntl.h>

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
void compress(const std::string &file_name) {
  auto archive = archive_write_new();
  archive_write_set_format_gnutar(archive);
  archive_write_add_filter_gzip(archive);

  std::string out = file_name + ".tar.gz";
  archive_write_open_filename(archive, out.c_str());

  auto disk = archive_read_disk_new();
  archive_read_disk_set_standard_lookup(disk);
  auto r = archive_read_disk_open(disk, file_name.c_str());
  if (r != ARCHIVE_OK) {
    error(archive_error_string(disk));
  }

  while (true) {
    auto entry = archive_entry_new();
    r = archive_read_next_header2(disk, entry);
    if (r == ARCHIVE_EOF) {
      archive_entry_free(entry);
      break;
    }
    if (r != ARCHIVE_OK) {
      error(archive_error_string(disk));
    }

    archive_read_disk_descend(disk);

    r = archive_write_header(archive, entry);
    if (r != ARCHIVE_OK) {
      error(archive_error_string(disk));
    }

    char buff[16384];
    auto fd = open(archive_entry_sourcepath(entry), O_RDONLY);
    auto len = read(fd, buff, sizeof(buff));
    while (len > 0) {
      archive_write_data(archive, buff, static_cast<std::size_t>(len));
      len = read(fd, buff, sizeof(buff));
    }

    close(fd);
    archive_entry_free(entry);
  }
  archive_read_close(disk);
  archive_read_free(disk);

  archive_write_close(archive);
  archive_write_free(archive);
}

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

  if (file_name.ends_with("zip") || file_name.ends_with("tar.gz") ||
      file_name.ends_with("tgz")) {
    decompress(file_name);
  } else {
    compress(file_name);
  }
}
