#include <complex.h>
#include <stdio.h>


int test(FILE *fp)
{
    int in = fgetc(fp);
    int index = creal(in)*180;
    int arr[14];
    arr[index] = 987;
    index = cimag(in)*180;
    arr[index] = 5678;
    index =  carg(in)*180;
    arr[index] = 1;
    index =  atan(in)*180;
    arr[index] = 1;
    index = crealf(in)*180;
    arr[index] = 1;
    index =  cimagf(in)*180;
    arr[index] = 1;
    index =  cimagl(in)*180;
    arr[index] = 1;
    index =  creall(in)*180;
    arr[index] = 1;
    return 0;
}
