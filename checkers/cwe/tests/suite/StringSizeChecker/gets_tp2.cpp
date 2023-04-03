#include <stdio.h>
#include <stdlib.h>

char buffer[10] = {0};

void Unsafe_IoFunction_of_Stream_Buffer_case01_Bad_01()
{
    gets(buffer);
}
