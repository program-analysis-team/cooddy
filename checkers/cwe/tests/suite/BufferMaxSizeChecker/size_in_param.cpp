int strncat_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

int foo(int some_size)
{
    char dstBuff[20];
    int s1 = 10;
    int size = s1 + some_size;
    strncat_s(dstBuff, sizeof(dstBuff), "Some data", size);
}

int main()
{
    foo(20);
}
