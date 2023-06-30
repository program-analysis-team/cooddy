int memcpy_s(void* dstBuff, int dstMax, const void* srcBuff, int srcSize);
void HdcFastRecv(int* a);

void Detect()
{
    int ctrlBuf[20000];
    HdcFastRecv(ctrlBuf);
    int size = ctrlBuf[10];
    char dstBuff[20000];
    char srcBuff[20000];
    memcpy_s(dstBuff, size + 0, dstBuff, size);  // DETECT
}

void Detect2()
{
    int ctrlBuf[20000];
    HdcFastRecv(ctrlBuf);
    int size = ctrlBuf[10];
    char dstBuff[20000];
    char srcBuff[20000];
    memcpy_s(dstBuff, size, srcBuff, size); // DETECT
}
