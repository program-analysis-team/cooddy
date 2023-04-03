#include <cstdlib>
#include <fstream>
void f1()
{
    int a[10];
    std::ifstream ifs("file");
    ifs >> a[5];

    for (int i = 0; i < a[5]; i++) {  // Loop here
    }
}
