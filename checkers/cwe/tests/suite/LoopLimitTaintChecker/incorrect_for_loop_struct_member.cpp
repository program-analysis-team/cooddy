#include <cstdlib>
#include <fstream>

struct A {
    int i;
};

void f1()
{
    A a;
    std::ifstream ifs("file");
    ifs >> a.i;

    for (int i = 0; i < a.i; i++) {  // Loop here
    }
}
