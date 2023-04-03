#include <stdio.h>
#include <stdlib.h>

typedef unsigned int UINT32;
#define MAX_SHIFT_COUNT 64
#define OK 1
#define ERROR 0

typedef struct {
    UINT32 offset;
} StructInfo;

typedef struct {
    UINT32 dataLen;
    char* dataBuff;
} DataInfo;

typedef struct {
    UINT32 offset;
    UINT32 dataLen;
    char* dataBuff;
} SendDataInfo;

typedef struct {
    StructInfo* info;
} DataInfoStruct;

void GetDataBuffInfo(DataInfo* dataBuffInfo, UINT32 inputNum)
{
    if (inputNum != MAX_SHIFT_COUNT) {
        dataBuffInfo->dataBuff = (char*)malloc(MAX_SHIFT_COUNT);
        dataBuffInfo->dataLen = MAX_SHIFT_COUNT;
        if (dataBuffInfo->dataBuff == NULL) {
            return;
        }
    }
}

void PrintBuf(SendDataInfo* dataBuffInfo)
{
    for (UINT32 i = 0; i < dataBuffInfo->dataLen; i++) {
        printf("buf data: %c", *(dataBuffInfo->dataBuff + i));
    }
}

void SaveBufInfo(DataInfo* dataBuffInfo)
{
    SendDataInfo* sendData = NULL;
    sendData = (SendDataInfo*)malloc(sizeof(SendDataInfo));
    if (sendData == NULL) {
        return;
    }

    sendData->offset = MAX_SHIFT_COUNT;
    sendData->dataBuff = dataBuffInfo->dataBuff;
    sendData->dataLen = dataBuffInfo->dataLen;
    PrintBuf(sendData);
    free(sendData);
}

void NotifyBuff(DataInfo* dataBuffInfo, UINT32 inputNum2)
{
    switch (inputNum2) {
        case MAX_SHIFT_COUNT:
            SaveBufInfo(dataBuffInfo);
            break;
        default:
            break;
    }

    if (dataBuffInfo->dataBuff != NULL) {
        free(dataBuffInfo->dataBuff);
    }
}

void WB_BadCase02(UINT32 inputNum1, UINT32 inputNum2)
{
    DataInfo dataBuffInfo = {0};
    GetDataBuffInfo(&dataBuffInfo, inputNum1);
    NotifyBuff(&dataBuffInfo, inputNum2);
}
