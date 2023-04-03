#include <cstdlib>
#include <fstream>

void f1()
{
    int x = 0;
    std::ifstream ifs("file");
    ifs >> x;
    if(x > 10)
        x = 10;
    for (int i = 0; i < x; i++) { // No loop here
    }
}
