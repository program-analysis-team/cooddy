#include <stdio.h>
int main()
{
    FILE* pFile;
    pFile = fopen("myfile.txt", "w");

    fputs("fopen example", pFile);
    fclose(pFile);

    return 0;
}
