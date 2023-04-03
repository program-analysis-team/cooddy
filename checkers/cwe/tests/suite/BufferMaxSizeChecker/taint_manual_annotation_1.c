int memcpy_s(void* dstBuff, int dstMax, const void* srcBuff, int srcSize);

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

static void EXTER_ATTACK foo(const char* str, int size)
{
    char dstBuff[20];
    memcpy_s(dstBuff, sizeof(dstBuff), str, size);
}
