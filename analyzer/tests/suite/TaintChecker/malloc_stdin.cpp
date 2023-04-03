#include <cstdlib>
#include <iostream>

void f1()
{
    int arr[10] = {};

    int x = 0;
    std::cin >> x;

    std::malloc(x);
}
