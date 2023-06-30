#include <math.h>
#include <stdio.h>


int test(FILE *fp)
{
    int in = fgetc(fp);
    int index = asin(in)*180;
    int arr[14];
    arr[index] = 987;
    index = asinf(in)*180;
    arr[index] = 5678;
    index =  atanf(in)*180;
    arr[index] = 1;
    index =  atan(in)*180;
    arr[index] = 1;
    index =  cacos(in)*180;
    arr[index] = 1;
    index =  cacosf(in)*180;
    arr[index] = 1;
    index =  casin(in)*180;
    arr[index] = 1;
    index =  casinf(in)*180;
    arr[index] = 1;
    return 0;
}
