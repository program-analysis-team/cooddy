typedef struct {
    unsigned dataLen;
    char* dataBuff;
} DataInfo1;

typedef struct {
    int dataLen;
    char* dataBuff;
} DataInfo2;

void foo()
{
    DataInfo1 dataBuffInfo1 = {0};
    auto x1 = *dataBuffInfo1.dataBuff; // TP here

    DataInfo2 dataBuffInfo2 = {0};
    auto x2 = *dataBuffInfo2.dataBuff; // TP here
}
