#include <stdio.h>
#include <stdlib.h>

void print(const int* data, int len)
{
    for (int i = 0; i < len; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
}

int* swap(int* in, int len, int pos)
{
    int x = *(in + len - pos);
    return in;
}

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK main(int argc, char* argv[])
{
    if (argc < 2)
        return -1;
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    print(array, 10);
    print(swap(array, 10, atoi(argv[1])), 10);
    return 0;
}
