
#include "string.h"

int main()
{
    char src[6] = "Hello";
    char dest[5] = {};
    strcpy(dest, src);  // Off-by-one while writing null-terminator behind buffer
    return 0;
}
