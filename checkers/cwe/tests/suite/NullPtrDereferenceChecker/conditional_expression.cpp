#include <stdlib.h>

char local_getline(char *zLine)
{
    int nLine = zLine == 0 ? 0 : 100;
    int n = 0;

    if (n + 100 > nLine) {
        nLine = nLine * 2 + 100;
        zLine = (char *)realloc(zLine, nLine);
    }
    return zLine != nullptr ? *zLine : 'a';
}

int main()
{
    local_getline(0);
    return 0;
}
