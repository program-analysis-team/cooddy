#include "header.h"

int g(bool condition)
{
    int arr[10];

    int index = *f(condition);

    return arr[index];
}

int main(int argc, char** argv)
{
    g(argc);

    int* x = nullptr;
    return *x;
}
