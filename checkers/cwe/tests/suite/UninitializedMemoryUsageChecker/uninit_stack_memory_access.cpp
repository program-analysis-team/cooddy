#include <stdio.h>
#include <stdlib.h>

void uninit_memory_access_001()
{
    unsigned long a;
    unsigned long *ret;
    ret = &a;
    printf("%ld ", *ret); /*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
}
