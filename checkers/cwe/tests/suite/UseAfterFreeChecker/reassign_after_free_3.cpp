#include <iostream>

void foo(int **p)
{
    delete *p;
    *p = new int(0xBBBB);
}

int main()
{
    int *p = new int(0xAAAAA);
    foo(&p);
    int x = *p;
    delete p;
    return x;
}
