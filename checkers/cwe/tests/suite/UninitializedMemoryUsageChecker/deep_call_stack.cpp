#include <stdint.h>

typedef void (*BLOCK_DB_ARRT_CB_FN)(void *arg);

void getBlockObjDbAttrSyncCb(void *arg);
void init(void *arg);

void getBlockObjDbAttrAsyncInner(char *tbName, char *key, char *attr, void *attrBuf, uint32_t notNullattrLen,
                                 BLOCK_DB_ARRT_CB_FN callback)
{
    if (tbName == nullptr || key == nullptr || attr == nullptr || attrBuf == nullptr || callback == nullptr ||
        notNullattrLen == 0) {
        if (callback != nullptr) {
            callback(attrBuf);
        }
        return;
    }
    init(attrBuf);
}

int32_t getBlockObjDbAttrSyncInner(char *tbName, char *key, char *attr, void *attrBuf, uint32_t notNullattrLen)
{
    uint32_t retryTimes = 3;
    while (retryTimes > 0) {
        getBlockObjDbAttrAsyncInner(tbName, key, attr, attrBuf, notNullattrLen, getBlockObjDbAttrSyncCb);
        retryTimes--;
    }
    return 0;
}

int32_t getBlockObjDbAttrSync(char *tbName, char *key, char *attr, void *attrBuf, uint32_t notNullattrLen)
{
    return getBlockObjDbAttrSyncInner(tbName, key, attr, attrBuf, notNullattrLen);
}

int32_t getLunDbRecAttr(uint32_t lunId, char *attr, void *attrBuf, uint32_t notNullattrLen)
{
    char objKey[123];
    int32_t retVal = getBlockObjDbAttrSync("tbName", objKey, attr, attrBuf, notNullattrLen);
}

int32_t getLunDbRecStatus(uint32_t lunId, uint8_t *status)
{
    return getLunDbRecAttr(lunId, "attr", status, sizeof(uint8_t));
}

uint8_t createCdpCheckSrcLunPara(uint32_t srcLunId)
{
    uint8_t status;
    int32_t ret = getLunDbRecStatus(srcLunId, &status);
    if (ret == 0) {
        return status + 1;
    }
    return 0;
}
