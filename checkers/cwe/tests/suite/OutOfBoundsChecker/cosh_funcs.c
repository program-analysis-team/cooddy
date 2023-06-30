#include <math.h>
#include <stdio.h>


int test(FILE *fp)
{
    int in = fgetc(fp);
    int index = asinh(in)*180;
    int arr[14];
    arr[index] = 987;
    index = asinhf(in)*180;
    arr[index] = 5678;
    index =  atanhf(in)*180;
    arr[index] = 1;
    index =  atanh(in)*180;
    arr[index] = 1;
    index =  cacosh(in)*180;
    arr[index] = 1;
    index =  cacoshf(in)*180;
    arr[index] = 1;
    index =  casinh(in)*180;
    arr[index] = 1;
    index =  casinhf(in)*180;
    arr[index] = 1;
    return 0;
}
