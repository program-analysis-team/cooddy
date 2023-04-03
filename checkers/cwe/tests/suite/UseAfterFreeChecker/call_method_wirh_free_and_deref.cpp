#include <stdlib.h>

void func(int* p)
{
    *p = 10;
    free(p);
}

int main()
{
    int* a = (int*)calloc(100, sizeof(int));
    func(a);
    return 0;
}
