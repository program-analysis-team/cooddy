
int memset_s(void* buff, int buffSize, char pattern, int count);

static void foo(char* buff, int size)
{
    memset_s(buff, size, 0, 100);
}
