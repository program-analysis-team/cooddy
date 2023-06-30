#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int uninit_pointer_009_func_001(int flag)
{
    int ret;
    if (flag == 0)
        ret = 0;
    else
        ret = flag + 1;
    return ret;
}

void uninit_pointer_009()
{
    char *buf, buf1[25];
    int flag = 10;

    if (uninit_pointer_009_func_001(flag) == 0) {
        buf = "This is a string";
    }
    if (uninit_pointer_009_func_001(flag) > 0) {
        strcpy(buf1, buf); /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
    }
}
