#include <stdlib.h>

int func1(int a, int* b)
{
    a += 5;
    if (a <= 10)
        return *b;
    return 5;
}

int main()
{
    int* p = nullptr;
    int c = 2;
    func1(c * 3, p);
    return 0;
}
