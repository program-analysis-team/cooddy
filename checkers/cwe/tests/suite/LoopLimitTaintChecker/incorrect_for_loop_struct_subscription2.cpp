#include <cstdlib>
#include <fstream>

struct A {
    int* i;
};

void f1()
{
    A a[10];
    std::ifstream ifs("file");
    ifs >> a[4].i[5];

    for (int i = 0; i < a[4].i[5]; i++) {  // Loop here
    }
}
