#include <stdio.h>

typedef struct {
    int age;
    int name;
} P;

typedef struct {
    int cnt;
    int type;
    P* buff;
} DataInfo;

static void PrintDataBuff(DataInfo* dataInfo)
{
    if (!dataInfo) {
        return;
    }
    printf("%s", dataInfo->buff->name);  // Null pointer dereference
}

static void FORWARD_NULL_11_BadCase001()
{
    DataInfo dataInfo = {0};
    dataInfo.cnt = 1;
    dataInfo.type = 0;
    PrintDataBuff(&dataInfo);
}
