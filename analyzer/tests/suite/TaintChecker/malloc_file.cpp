#include <cstdlib>
#include <fstream>

void f1()
{
    int x = 0;
    std::ifstream ifs("file");
    ifs >> x;

    std::malloc(x);
}
