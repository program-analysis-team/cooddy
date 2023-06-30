#include <stdio.h>

int GetSize(int x)
{
    return x;
}

int main()
{
    int (*Size)(int) = GetSize;

    printf("%d", Size(5));
}
