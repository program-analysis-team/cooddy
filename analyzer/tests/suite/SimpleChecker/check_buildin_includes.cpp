#include <cassert>
int buildin_error();

int _main() {
#ifndef assert
  return buildin_error();
#else
  return 0;
#endif
}
