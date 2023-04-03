#include <stdio.h>
#include <stdlib.h>

void Unsafe_IoFunction_of_Stream_Buffer_case01_Bad_01()
{
    char buffer[10] = {0};
    gets(buffer);
}
