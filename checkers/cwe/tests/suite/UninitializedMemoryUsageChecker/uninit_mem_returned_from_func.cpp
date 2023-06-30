#include <stdlib.h>

#include <cstring>

int (*uninit_memory_access_015_func_001())[4]
{
    int(*p)[4];
    int arr[4][4];
    int i, j;
    p = (int(*)[4])malloc(sizeof arr);
    memcpy(p, arr, sizeof(arr));
    for (i = 0; i < 4; i++) {
        for ( j=0 ;j<4; j++)
        {
            *(p[i]+j) += *(p[i]+j);/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
        }
    }
    return p;
}

void uninit_memory_access_015 ()
{
    int (*ptr)[4];
    ptr = uninit_memory_access_015_func_001();
    free(ptr);
    ptr = NULL;
}
