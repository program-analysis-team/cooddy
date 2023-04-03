#include <cstdlib>
#include <fstream>

void f1()
{
    int x = 0;
    std::ifstream ifs("file");
    ifs >> x;

    for (int i = 0; i < x; i++) { // Loop here
    }
}
