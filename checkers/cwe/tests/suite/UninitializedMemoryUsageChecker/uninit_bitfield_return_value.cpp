#include <stdio.h>
#include <stdlib.h>


/*
 * Types of defects: Uninitialized Memory Access
 * Complexity: When using a pointer to ( signed) bit field variable ,initialized by return value of function
 */
typedef struct {
    signed int a : 7;
    signed int b : 7;
} uninit_memory_access_008_s_001;

uninit_memory_access_008_s_001 *uninit_memory_access_008_func_001()
{
    uninit_memory_access_008_s_001 *ret =
        (uninit_memory_access_008_s_001 *)malloc(5 * sizeof(uninit_memory_access_008_s_001));
    return ret;
}

void uninit_memory_access_008()
{
    uninit_memory_access_008_s_001 *s = NULL;
    s = uninit_memory_access_008_func_001();
    s->b = s->a; /*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
}
