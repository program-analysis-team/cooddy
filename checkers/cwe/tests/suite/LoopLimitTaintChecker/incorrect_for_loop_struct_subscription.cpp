#include <cstdlib>
#include <fstream>

struct A {
    int* i;
};

void f1()
{
    A a;
    std::ifstream ifs("file");
    ifs >> a.i[5];

    for (int i = 0; i < a.i[5]; i++) {  // Loop here
    }
}
