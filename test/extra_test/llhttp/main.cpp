#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <llhttp.h>

struct Request {
  std::string url_;
  std::vector<std::pair<std::string, std::string>> field_;
};

std::int32_t handle_url(llhttp_t* parser, const char* at, std::size_t length) {
  auto data = static_cast<Request*>(parser->data);
  data->url_.assign(at, length);
  return 0;
}

std::int32_t handle_header_field(llhttp_t* parser, const char* at,
                                 std::size_t length) {
  auto data = static_cast<Request*>(parser->data);
  data->field_.emplace_back(std::string(at, length), "");
  return 0;
}

std::int32_t handle_header_value(llhttp_t* parser, const char* at,
                                 std::size_t length) {
  auto data = static_cast<Request*>(parser->data);
  data->field_.back().second = std::string(at, length);
  return 0;
}

int main() {
  llhttp_settings_t settings;
  llhttp_settings_init(&settings);
  settings.on_url = handle_url;
  settings.on_header_field = handle_header_field;
  settings.on_header_value = handle_header_value;

  llhttp_t parser;
  llhttp_init(&parser, HTTP_REQUEST, &settings);

  Request request;
  parser.data = &request;

  auto request_header =
      "POST /myform.html HTTP/1.1\r\n"
      "Host: developer.mozilla.org\r\n"
      "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:50.0) "
      "Gecko/20100101 Firefox/50.0\r\n"
      "Content-Length: 128\r\n\r\n";

  auto err =
      llhttp_execute(&parser, request_header, std::strlen(request_header));
  if (err == HPE_OK) {
    std::cout << "method: "
              << llhttp_method_name(static_cast<llhttp_method_t>(parser.method))
              << '\n';

    std::cout << "url: " << request.url_ << '\n';

    std::cout << "HTTP/" << static_cast<std::int32_t>(parser.http_major) << '.'
              << static_cast<std::int32_t>(parser.http_minor) << '\n';

    for (const auto& [key, value] : request.field_) {
      std::cout << key << ": " << value << '\n';
    }
  } else {
    std::cerr << "Parse error: " << llhttp_errno_name(err) << ": "
              << parser.reason << '\n';
  }
}
