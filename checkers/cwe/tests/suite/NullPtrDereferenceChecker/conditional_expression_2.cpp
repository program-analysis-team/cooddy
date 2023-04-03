#include <stdlib.h>

char local_getline(char *zLine)
{
    return zLine != 0 ? *zLine : 100;
}

int main()
{
    local_getline(0);
    return 0;
}
