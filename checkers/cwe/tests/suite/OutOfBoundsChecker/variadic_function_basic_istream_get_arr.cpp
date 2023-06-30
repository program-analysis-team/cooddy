#include <iostream>

void foo()
{
    char buf[12];
    int arr[14];
    std::cin.get(buf, 12);
    arr[buf[1]] = 1;
    std::cin.get(buf, 12, ' ');
    arr[buf[2]] = 1;
}
