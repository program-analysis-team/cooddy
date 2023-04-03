#include <stdio.h>

struct S {};

int main()
{
    S s{};
    scanf("%p", &s);
}
