char g_array[50];

int memcpy_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

void bar(char* arr, int size)
{
    char buf[100];
    int s = (size < 100 ? size : 100);
    memcpy_s(buf, sizeof(buf), arr, s);
}

void foo(int size)
{
    bar(g_array, size);
}
