#include <cstdlib>
#include <iostream>

#include <Python.h>

// https://docs.python.org/zh-cn/3/extending/embedding.html#very-high-level-embedding
int main(int argc, const char *argv[]) {
  (void)argc;

  auto program = Py_DecodeLocale(argv[0], nullptr);
  if (program == nullptr) {
    std::cerr << "Fatal error: cannot decode argv[0]\n";
    std::exit(EXIT_FAILURE);
  }

  Py_SetProgramName(program);

  std::cout << Py_GetVersion() << std::endl;

  Py_Initialize();

  PyRun_SimpleString(R"(
from time import time,ctime
print('Today is', ctime(time()))
)");

  if (Py_FinalizeEx() < 0) {
    std::cerr << "Py_FinalizeEx() failed\n";
    std::exit(EXIT_FAILURE);
  }
  PyMem_RawFree(program);
}
