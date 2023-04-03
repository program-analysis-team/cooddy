#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TestCaseBad02()
{
    int n = 0;
    int ret = scanf_s("%d", &n);
    char *p = (char*)malloc(n);
    if (p == NULL) {
        return;
    }
    p[n] = 'a';
    free(p);
}
