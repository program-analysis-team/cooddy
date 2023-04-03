#include <cstdlib>
#include <iostream>

void f1()
{
    int arr[10] = {};

    int x = 0;
    std::cin >> x;
    if(x > 10)
        x = 10;
    for (int i = 0; i < x; i++) { // No loop here
    }
}
