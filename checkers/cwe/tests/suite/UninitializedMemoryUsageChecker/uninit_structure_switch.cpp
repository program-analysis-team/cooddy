#include <stdio.h>
#include <stdlib.h>

typedef union {
    int a;
    int b;
} uninit_memory_access_014_u_001;


uninit_memory_access_014_u_001 * uninit_memory_access_014_func_001 ()
{
    int flag = rand();
    uninit_memory_access_014_u_001 *u;
    switch (flag)
    {
        case 1:
        {
            u = (uninit_memory_access_014_u_001 *)calloc(1,sizeof(uninit_memory_access_014_u_001));
            u->a = 40;
            return u;
        }
        case 2:
        {
            u = (uninit_memory_access_014_u_001 *)calloc(2,sizeof(uninit_memory_access_014_u_001));
            u->a = 20;
            return u;
        }
        case 3:
        {
            u = (uninit_memory_access_014_u_001 *)calloc(3,sizeof(uninit_memory_access_014_u_001));
            u->a = 30;
            return u;
        }
        default:
            return (uninit_memory_access_014_u_001 *)(-1);
    }
}

void uninit_memory_access_014 ()
{
    int ret;
    uninit_memory_access_014_u_001 *p;
    p = uninit_memory_access_014_func_001 ();
    if(p != NULL)
    {
        ret = p->b;/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
        free(p);
        p= NULL;
    }
}
