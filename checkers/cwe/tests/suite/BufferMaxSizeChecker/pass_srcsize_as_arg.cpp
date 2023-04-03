extern "C" {
    int memset_s(char* buff, int buffSize, char pattern, int count);
}

void init(char * buff, int size, int count) {
    memset_s(buff, size, 0, count);
}

int main()
{
    char dstBuff[20];
    init(dstBuff, sizeof(dstBuff), 21);
    return 0;
}
