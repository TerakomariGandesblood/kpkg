#include <cstdlib>
#include <iostream>

#include <sqlite3.h>

int main() {
  sqlite3 *db;

  if (sqlite3_open("test.db", &db) != 0) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << '\n';
    return EXIT_FAILURE;
  }

  std::clog << "Opened database successfully\n";
  sqlite3_close(db);
}
