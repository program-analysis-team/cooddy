#include <stdio.h>

#include <iostream>
#include <string>

int main()
{
    wchar_t buff[100] = L"Hello";
    FILE *f = fopen("test.txt", "r");
    if (f != nullptr) {
        fread(buff, 1, sizeof(buff), f);
        fclose(f);
    }
    wprintf(buff, 10);
    return 0;
}
