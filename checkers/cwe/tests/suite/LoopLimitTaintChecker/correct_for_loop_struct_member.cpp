#include <cstdlib>
#include <fstream>
struct B {
    int i;
    int j;
};
struct A {
    B* b;
};

void f1()
{
    A* a;
    std::ifstream ifs("file");
    a->b->j = 10;
    ifs >> a->b->i;

    for (int i = 0; i < a->b->j; i++) {  // Loop here
    }
}
