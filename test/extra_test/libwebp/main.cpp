#include <iostream>

#include <webp/encode.h>

int main() { std::cout << std::hex << WebPGetEncoderVersion() << '\n'; }
