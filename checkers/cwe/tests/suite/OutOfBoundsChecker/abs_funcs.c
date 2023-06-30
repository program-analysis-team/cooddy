#include <ctype.h>
#include <stdio.h>


int test(FILE *fp)
{
    int in = fgetc(fp);
    int index = abs(in);
    int arr[14];
    arr[index] = 987;
    index = labs(in);
    arr[index] = 5678;
    index =  llabs(in);
    arr[index] = 1;
    index =  fabs(in);
    arr[index] = 1;
    index =  fabsf(in);
    arr[index] = 1;
    index =  fabsl(in);
    arr[index] = 1;
    return 0;
}
