#include <stdlib.h>
#include <fstream>

int test()
{
    std::ifstream file("some_external.txt");
    short index;
    file >> index;
    int arr[1234];
    arr[index] = 987;
    return 0;
}
