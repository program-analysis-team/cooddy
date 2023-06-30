#include <ctype.h>
#include <stdio.h>


int test(FILE *fp)
{
    int in = fgetc(fp);
    int index = tolower(in);
    int arr[14];
    arr[index] = 987;
    index = toupper(in);
    arr[index] = 5678;
    index =  toascii(in);
    arr[index] = 1;
    return 0;
}
