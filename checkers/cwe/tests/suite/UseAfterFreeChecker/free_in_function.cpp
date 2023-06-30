#include <iostream>

void foo(int **p)
{
    delete *p;
}

int main()
{
    int *p = new int(0xAAAAA);
    foo(&p);
    return *p;
}
