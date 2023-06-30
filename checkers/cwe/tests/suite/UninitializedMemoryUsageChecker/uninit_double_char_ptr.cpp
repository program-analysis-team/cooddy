#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void uninit_pointer_007 ()
{
    char *buf1=strdup("String1");
    char *buf2;
    char *buf3=strdup("String3");
    char *buf4=strdup("String4");
    char *buf5=strdup("String5");
    if (!buf1 || !buf3 || !buf4 || !buf5) return;
    char **pbuf[5] = {&buf2, &buf3, &buf4, &buf5};
    int i,j=4;

    for(i=0;i<5;i++)
    {
        *((*pbuf[i])+j)='a';/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
    }
    free(buf1);
    free(buf3);
    free(buf4);
    free(buf5);
}
