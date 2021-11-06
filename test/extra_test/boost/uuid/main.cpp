#include <iostream>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

int main() {
  auto uuid = boost::uuids::random_generator()();
  std::cout << boost::uuids::to_string(uuid) << '\n';
}
