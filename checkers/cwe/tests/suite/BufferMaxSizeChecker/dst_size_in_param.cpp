#define MAX_BUFF_SIZE 10

int strncpy_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);

void foo(char* buff, int buffSize)
{
    strncpy_s(buff, buffSize, "Some data", MAX_BUFF_SIZE);
}
