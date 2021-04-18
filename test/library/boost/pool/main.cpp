#include <string>

#include <boost/pool/pool_alloc.hpp>

#include "error.h"

// https://www.boost.org/doc/libs/1_76_0/libs/pool/doc/html/index.html
class Test {
 public:
  explicit Test(const std::string& str) : str_(str) {}
  [[nodiscard]] const std::string& get_string() const { return str_; }

 private:
  std::string str_;
};

int main() {
  boost::fast_pool_allocator<Test> pool;
  auto p = pool.allocate(1);
  pool.construct(p, "str");

  EXPECT(p->get_string() == "str");

  pool.destroy(p);
  pool.deallocate(p, 1);
}
