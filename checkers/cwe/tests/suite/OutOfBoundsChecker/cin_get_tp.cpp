#include <iostream>

int test()
{
    long long index = std::cin.get();
    int arr[14];
    arr[index] = 987;
    index = std::cin.tellg();
    arr[index] = 5678;
    index = std::cin.fill();
    arr[index] = 1;
    index = std::cin.narrow('b','c');
    arr[index] = 132;
    index = std::cin.narrow('d','e');
    arr[index] = 148;
    return 0;
}
