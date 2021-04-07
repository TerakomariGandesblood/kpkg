find_package(RocksDB REQUIRED)
add_executable(rocksdb main.cpp)
target_link_libraries(rocksdb RocksDB::rocksdb)
