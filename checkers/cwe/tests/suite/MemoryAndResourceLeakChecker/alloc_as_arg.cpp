#include <iostream>

void foo(int *x){delete x;}

void baz()
{
    foo(new int(42));
}
