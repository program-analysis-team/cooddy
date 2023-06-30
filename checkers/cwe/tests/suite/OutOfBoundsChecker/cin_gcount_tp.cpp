#include <iostream>

int test()
{
    short index = std::cin.gcount();
    int arr[1234];
    arr[index] = 987;
    return 0;
}
