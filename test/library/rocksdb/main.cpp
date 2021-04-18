#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <rocksdb/db.h>
#include <rocksdb/iterator.h>
#include <rocksdb/write_batch.h>

#include "error.h"

// https://rocksdb.org/docs/getting-started.html
int main() {
  rocksdb::DB* p = nullptr;
  rocksdb::Options options;
  options.create_if_missing = true;
  // options.error_if_exists = true;
  auto status = rocksdb::DB::Open(options, "testdb", &p);
  std::unique_ptr<rocksdb::DB> db(p);

  if (!status.ok()) {
    std::cerr << status.ToString() << '\n';
    return EXIT_FAILURE;
  }

  rocksdb::WriteBatch batch;
  batch.Put("name", "kaiser");
  status = db->Write(rocksdb::WriteOptions(), &batch);
  if (!status.ok()) {
    std::cerr << status.ToString() << '\n';
    return EXIT_FAILURE;
  }

  std::string value;
  status = db->Get(rocksdb::ReadOptions(), "name", &value);
  if (!status.ok()) {
    std::cerr << status.ToString() << '\n';
    return EXIT_FAILURE;
  }
  EXPECT(value == "kaiser");

  status = db->Delete(rocksdb::WriteOptions(), "name");
  if (!status.ok()) {
    std::cerr << status.ToString() << '\n';
    return EXIT_FAILURE;
  }

  auto iter = db->NewIterator(rocksdb::ReadOptions());
  iter->SeekToFirst();
  EXPECT(!iter->Valid());
}
