#include <stdio.h>

int main()
{
    FILE* pFile = nullptr;
    char name [100];
    fprintf (pFile, "Name %d [%-10.10s]\n",2,name);
    return 0;
}
