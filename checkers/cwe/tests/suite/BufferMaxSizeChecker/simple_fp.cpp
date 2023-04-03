int memcpy_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

int main()
{
    char dstBuff[20];
    char srcBuff[20] = "Some data";
    memcpy_s(dstBuff, sizeof(dstBuff), srcBuff, sizeof(srcBuff));
    return 0;
}
