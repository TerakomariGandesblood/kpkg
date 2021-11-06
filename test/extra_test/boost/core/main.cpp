#include <cstdint>

#include <boost/core/ignore_unused.hpp>
#include <boost/core/noncopyable.hpp>

class A : public boost::noncopyable {
 public:
  A() = default;
};

int main() {
  std::int32_t num = 10;
  boost::ignore_unused(num);

  A a;
  boost::ignore_unused(a);
}
