#include <iostream>
#include <string>

#include <CLI/CLI.hpp>

// https://cliutils.github.io/CLI11/book/
int main(int argc, char *argv[]) {
  CLI::App app("kpkg, a C++ library manager");
  app.set_version_flag("-v,--version", "version 0.1.0");
  app.require_subcommand(1);

  auto install = app.add_subcommand("install", "Install library");

  std::string proxy;
  install
      ->add_flag("-p{http://127.0.0.1:1080},--proxy{http://127.0.0.1:1080}",
                 proxy, "Use proxy")
      ->expected(0, 1);

  std::vector<std::string> libraries;
  install->add_option("library", libraries, "The library to be installed")
      ->required();

  install->callback([&] {
    if (std::empty(proxy)) {
      std::cout << "No proxy\n";
    } else {
      std::cout << "Use proxy: " << proxy << '\n';
    }

    std::cout << "lib:";
    for (const auto &lib : libraries) {
      std::cout << ' ' << lib;
    }

    std::cout << '\n';
  });

  auto list = app.add_subcommand("list", "Show all libraries");
  list->add_flag("-p{http://127.0.0.1:1080},--proxy{http://127.0.0.1:1080}",
                 proxy, "Use proxy")
      ->expected(0, 1);
  list->callback([&] {
    if (std::empty(proxy)) {
      std::cout << "No proxy:";
    } else {
      std::cout << "Use proxy: " << proxy;
    }
  });

  CLI11_PARSE(app, argc, argv);
}
