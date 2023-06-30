#include <math.h>
#include <stdio.h>


int test(FILE *fp)
{
    int in = fgetc(fp);
    int index = exp(in)*180;
    int arr[14];
    arr[index] = 987;
    index = exp2(in)*180;
    arr[index] = 5678;
    index =  log(in)*180;
    arr[index] = 1;
    index =  log1p(in)*180;
    arr[index] = 1;
    index =  log2(in)*180;
    arr[index] = 1;
    index =  logb(in)*180;
    arr[index] = 1;
    index =  ilogb(in)*180;
    arr[index] = 1;
    index =  pow(in, 3)*180;
    arr[index] = 1;
    index =  sqrt(in)*180;
    arr[index] = 1;
    index =  cbrt(in)*180;
    arr[index] = 1;
    return 0;
}
