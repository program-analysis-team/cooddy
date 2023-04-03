#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

 int main()
{
    wchar_t* buffer = (wchar_t*)malloc(1234);
    auto cx = swprintf(buffer, 100, L"The half of %d is %d", 80, 80 / 2);

    return cx;
}
