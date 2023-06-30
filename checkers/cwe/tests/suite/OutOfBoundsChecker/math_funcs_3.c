#include <math.h>
#include <stdio.h>


int test(FILE *fp)
{
    int in = fgetc(fp);
    int index = fmin(in,4);
    int arr[14];
    arr[index] = 987;
    index = fmax(in, 8);
    arr[index] = 5678;
    index = fminimum(in, 8);
    arr[index] = 1;
    index = fmaximum(in, 8);
    arr[index] = 1;
    index = fminimum_num(in, 8);
    arr[index] = 1;
    index = fmaximum_num(in, 8);
    arr[index] = 1;
    index = fminmag(in, 8);
    arr[index] = 1;
    index = fmaxmag(in, 8);
    arr[index] = 1;
    index = fminimum_mag(in, 8);
    arr[index] = 1;
    index = fminimum_mag_num(in, 8);
    arr[index] = 1;
    return 0;
}
