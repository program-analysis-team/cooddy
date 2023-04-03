int memcpy_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

int main()
{
    char dstBuff[20];
    char srcBuff[100] = "Some data";
    int size1 = sizeof(srcBuff);
    int size2 = 10;
    memcpy_s(dstBuff, sizeof(dstBuff), srcBuff, size1 + size2);
    return 0;
}
