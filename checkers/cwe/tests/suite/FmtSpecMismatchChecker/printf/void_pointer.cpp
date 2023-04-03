#include <stdio.h>

struct S {};

int main()
{
    int i;
    S s;
    void* vp;
    printf("%p", &i);
    printf("%p", &s);
    printf("%p", vp);
}
