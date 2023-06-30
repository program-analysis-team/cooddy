#include <stdio.h>
static const int STATIC_CONST_FIVE = 5;

void CWE192_type_cast_other_3_error_06()
{

    int b = 0x41424344;
    char *a;

    if(STATIC_CONST_FIVE==5)
    {
        a = &b;
        printf("%c\n", *a);
        return;
    }

}
