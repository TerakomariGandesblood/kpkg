#pragma once

#include <string>
#include <string_view>

namespace libkpkg {

enum class Sanitize { None, Memory, Thread };

void run_command(const std::string& cmd, const std::string& dir = "");

class Command {
 public:
  Command() = default;
  Command(std::string_view configuration, std::string_view build,
          std::string_view install)
      : configuration_(configuration), build_(build), install_(install) {}

  virtual ~Command() = default;

  void run(const std::string& dir = "") const;
  virtual void add_option(const std::string& key, const std::string& value) = 0;

 protected:
  void build_with_sanitize(Sanitize sanitize);

  virtual void add_flags(std::string_view c_flags, std::string_view cxx_flags);

  std::string configuration_;
  std::string build_;
  std::string install_;
};

class CMakeCommand : public Command {
 public:
  explicit CMakeCommand(Sanitize sanitize);
  void add_option(const std::string& key, const std::string& value) override;
};

class MakeCommand : public Command {
 public:
  explicit MakeCommand(Sanitize sanitize);
  void add_option(const std::string& key, const std::string& value) override;
};

class B2Command : public Command {
 public:
  explicit B2Command(Sanitize sanitize);
  void add_option(const std::string& key, const std::string& value) override;
  void add_flags(std::string_view c_flags, std::string_view cxx_flags) override;
};

}  // namespace libkpkg
