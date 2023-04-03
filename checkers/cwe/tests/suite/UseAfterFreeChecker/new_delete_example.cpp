#include <iostream>

int main ()
{
    int * a = new int(12);
    delete a;
    int b = *a;
    return b;
}
