#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void uninit_memory_access_003 ()
{
    char *str1 = (char *) calloc(25,sizeof(char));
    char *str2 ;
    if (str1!=NULL) {
        strcpy(str1, str2); /*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
        printf("%s %s\n", str1, str2);
        free(str1);
    }
}
