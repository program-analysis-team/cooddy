#include <fstream>

void f1()
{
    int arr[10] = {};

    int x = 0;
    std::ifstream ifs("file");
    ifs >> x;

    *(arr + x);
}
