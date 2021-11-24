#include <cstdlib>
#include <iostream>

#include <Python.h>

// TODO FIXME
#ifdef KPKG_SANITIZER
#include <sanitizer/lsan_interface.h>
#endif

// https://docs.python.org/zh-cn/3/extending/embedding.html#very-high-level-embedding
int main(int argc, const char *argv[]) {
  (void)argc;
#ifdef KPKG_SANITIZER
  __lsan::ScopedDisabler disabler;
#endif

  auto program = Py_DecodeLocale(argv[0], nullptr);
  if (program == nullptr) {
    std::cerr << "Fatal error: cannot decode argv[0]\n";
    std::exit(EXIT_FAILURE);
  }

  Py_SetProgramName(program);

  std::cout << Py_GetVersion() << std::endl;

  Py_Initialize();

  PyRun_SimpleString(R"(
import sys
sys.path.append('/usr/local/lib')
import font_tools
font_tools.to_subset_woff2('SourceHanSansSC-Bold.otf','SourceHanSansSC-Bold.woff2','你好世界,这是一个字体子集')
)");

  if (Py_FinalizeEx() < 0) {
    std::cerr << "Py_FinalizeEx() failed\n";
    std::exit(EXIT_FAILURE);
  }
  PyMem_RawFree(program);
}
