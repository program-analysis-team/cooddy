int strncat_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

int main()
{
    char dstBuff[20];
    int s1 = 10, s2 = 15;
    int size = s1 + s2;
    strncat_s(dstBuff, sizeof(dstBuff), "Some data", size);
}
