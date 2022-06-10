#include <httplib.h>

#include <iostream>

int main() {
  httplib::Client cli("http://cpp-httplib-server.yhirose.repl.co");
  std::cout << cli.Get("/hi")->body << '\n';
}
