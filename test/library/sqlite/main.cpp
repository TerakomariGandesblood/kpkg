#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#include <sqlite3.h>

#include "error.h"

// https://zetcode.com/db/sqlitec/
// https://www.sqlite.org/docs.html
class Statement;

class Database {
  friend class Statement;

 public:
  explicit Database(const std::string &name) {
    if (sqlite3_open_v2(name.c_str(), &db_,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                        nullptr) != SQLITE_OK) {
      std::string msg =
          "Can't open database '" + name + "': " + sqlite3_errmsg(db_);
      // 无论是否出现错误, 都需要调用 sqlite3_close_v2
      sqlite3_close_v2(db_);
      throw std::runtime_error(msg);
    }
  }

  Database(const Database &) = delete;
  Database(Database &&) = delete;
  Database &operator=(const Database &) = delete;
  Database &operator=(Database &&) = delete;

  ~Database() { sqlite3_close_v2(db_); }

  void exec(std::string_view sql) {
    char *err_msg = nullptr;
    // sqlite3_exec 是对 sqlite3_prepare_v2(), sqlite3_step(), 和
    // sqlite3_finalize() 的一个包装
    if (sqlite3_exec(db_, sql.data(), nullptr, nullptr, &err_msg) !=
        SQLITE_OK) {
      auto msg = std::string("SQL error: ") + err_msg;
      sqlite3_free(err_msg);
      throw std::runtime_error(msg);
    }
    sqlite3_free(err_msg);
  }

  std::int64_t last_insert_rowid() const {
    return sqlite3_last_insert_rowid(db_);
  }

  static std::string version() { return sqlite3_libversion(); }

  [[nodiscard]] bool autocommit() const {
    // 默认下, SQLite 以 autocommit 模式运行
    // 在该模式下一个命令完成将立刻对数据库进行修改
    // 在执行 BEGIN 语句后禁用, 在执行 COMMIT 或 ROLLBACK 后启用
    return sqlite3_get_autocommit(db_);
  }

 private:
  sqlite3 *db_ = nullptr;
};

class Transaction {
 public:
  explicit Transaction(Database &database) : database_(database) {
    database_.exec("BEGIN TRANSACTION;");
  };

  Transaction(const Transaction &) = delete;
  Transaction(Transaction &&) = delete;
  Transaction &operator=(const Transaction &) = delete;
  Transaction &operator=(Transaction &&) = delete;

  ~Transaction() {
    try {
      if (!commited_) {
        database_.exec("ROLLBACK");
      }
    } catch (...) {
    }
  }

  void commit() {
    if (!commited_) {
      database_.exec("COMMIT");
      commited_ = true;
    } else {
      throw std::runtime_error("Transaction already committed");
    }
  }

 private:
  Database &database_;
  bool commited_ = false;
};

class Statement {
 public:
  Statement(const Database &database, std::string_view sql) {
    // 在 SQL 语句执行之前必须编译为 byte-code
    if (sqlite3_prepare_v2(database.db_, sql.data(),
                           static_cast<std::int32_t>(std::size(sql)), &res_,
                           nullptr) != SQLITE_OK) {
      auto msg =
          std::string("Failed to fetch data: ") + sqlite3_errmsg(database.db_);
      sqlite3_finalize(res_);
      throw std::runtime_error(msg);
    }
  }

  Statement(const Statement &) = delete;
  Statement(Statement &&) = delete;
  Statement &operator=(const Statement &) = delete;
  Statement &operator=(Statement &&) = delete;

  ~Statement() {
    // 销毁
    sqlite3_finalize(res_);
  }

  bool execute_step() {
    // SQLITE_ROW 表示有一行数据准备就绪
    return sqlite3_step(res_) == SQLITE_ROW;
  }

  void bind(std::int32_t index, std::int32_t value) {
    sqlite3_bind_int(res_, index, value);
  }

  void bind(std::string_view index, std::int32_t value) {
    sqlite3_bind_int(res_, sqlite3_bind_parameter_index(res_, index.data()),
                     value);
  }

  [[nodiscard]] std::string get_column(std::int32_t index) const {
    return reinterpret_cast<const char *>(sqlite3_column_text(res_, index));
  }

 private:
  sqlite3_stmt *res_ = nullptr;
};

int main() try {
  Database db("test.db");
  EXPECT(Database::version() == SQLITE_VERSION);
  EXPECT(db.autocommit());

  db.exec("DROP TABLE IF EXISTS Cars;");

  Transaction transaction(db);
  EXPECT(!db.autocommit());

  std::string_view sql =
      "CREATE TABLE Cars(Id INTEGER PRIMARY KEY, Name TEXT, Price INT);"
      "INSERT INTO Cars(Name, Price) VALUES('Audi', 52642);"
      "INSERT INTO Cars(Name, Price) VALUES('Mercedes', 57127);"
      "INSERT INTO Cars(Name, Price) VALUES('Skoda', 9000);"
      "INSERT INTO Cars(Name, Price) VALUES('Volvo', 29000);"
      "INSERT INTO Cars(Name, Price) VALUES('Bentley', 350000);"
      "INSERT INTO Cars(Name, Price) VALUES('Citroen', 21000);"
      "INSERT INTO Cars(Name, Price) VALUES('Hummer', 41400);"
      "INSERT INTO Cars(Name, Price) VALUES('Volkswagen', 21600);";
  db.exec(sql);

  transaction.commit();
  EXPECT(db.autocommit());
  EXPECT(db.last_insert_rowid() == 8);

  sql = "SELECT * FROM Cars";
  Statement query(db, sql);

  while (query.execute_step()) {
    std::cout << query.get_column(0) << ' ';
    std::cout << query.get_column(1) << ' ';
    std::cout << query.get_column(2) << '\n';
  }
} catch (const std::exception &err) {
  std::cerr << err.what() << '\n';
  return EXIT_FAILURE;
}
