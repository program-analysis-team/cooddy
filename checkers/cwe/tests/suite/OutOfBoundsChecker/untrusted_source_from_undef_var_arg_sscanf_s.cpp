#include <iostream>

void foo()
{
    char buf[2456];
    int arr[123];
    int a = fread(buf, 2456, 2456, fopen("file", "r"));
    arr[a] = 123;    // error here
    char str[123];
    int b = sscanf(buf, "%s", str, (rsize_t) sizeof(str));
    arr[b];          // error here
}
