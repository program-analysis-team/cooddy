#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int int_a;
    short short_b;
    short pad;
}uninit_memory_access_012_s_001;

void uninit_memory_access_012()
{
    uninit_memory_access_012_s_001  *s1, s;
    s1 = (uninit_memory_access_012_s_001*)calloc(1,sizeof(uninit_memory_access_012_s_001));
    if(s1 !=NULL)
    {
        s1->int_a = 10;
        memcpy(&s, s1, sizeof(uninit_memory_access_012_s_001));/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
        free(s1);
    }
}
