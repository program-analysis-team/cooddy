#include <cstdlib>
#include <fstream>
struct B {
    int i;
};
struct A {
    B* b;
};

void f1()
{
    A* a;
    std::ifstream ifs("file");
    ifs >> a->b->i;

    for (int i = 0; i < a->b->i; i++) {  // Loop here
    }
}
