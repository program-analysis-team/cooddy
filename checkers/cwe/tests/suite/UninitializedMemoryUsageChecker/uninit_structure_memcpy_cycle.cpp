#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int int_a;
    bool bool_b;
    bool dummy[3];
    int int_c;
}uninit_memory_access_013_s_001;

void uninit_memory_access_013 ()
{
    int i;
    uninit_memory_access_013_s_001 *s1, s;

    s1 = (uninit_memory_access_013_s_001*)calloc(1,sizeof(uninit_memory_access_013_s_001));
    s1->int_a = 10;
    s1->int_c = 20;

    for (i=0; i<1; i++)
        s1->dummy[i]= false;

    memcpy(&s, s1, sizeof(uninit_memory_access_013_s_001));/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
    free(s1);
}
