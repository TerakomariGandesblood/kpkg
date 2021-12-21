#include <stdexcept>

#include <backward.hpp>

backward::SignalHandling sh;

int main() { throw std::runtime_error("error"); }
