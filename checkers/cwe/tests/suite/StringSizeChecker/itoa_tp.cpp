char* itoa(int _Val, char* _DstBuf, int _Radix);

void test01_c(int src)
{
    char buf[10] = {0};
    char buf1[12] = {0};

    itoa(-2147483647, buf1, 10);  // no error
    itoa(src, buf, 10);           // error
}
