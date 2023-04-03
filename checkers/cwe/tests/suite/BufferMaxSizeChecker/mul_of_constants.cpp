int memmove_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

int main()
{
    char dstBuff[2];
    char srcBuff[] = "Some data";
    memmove_s(dstBuff, sizeof(dstBuff), srcBuff, 1 * sizeof(srcBuff));
    return 0;
}
