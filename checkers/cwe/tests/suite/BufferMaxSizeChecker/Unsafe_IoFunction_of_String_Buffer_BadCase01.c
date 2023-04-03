#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Unsafe_IoFunction_of_String_Buffer_case01_Bad_01(char* userinput)
{
    char cmd[20] = "/usr/bin/cat";
    strcat(cmd, userinput);
}
