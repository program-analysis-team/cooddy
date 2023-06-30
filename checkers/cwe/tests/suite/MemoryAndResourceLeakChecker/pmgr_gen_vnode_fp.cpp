#include <stdlib.h>

#include <cstdint>

static int32_t allocArgBuff(char **cfgTexBuf, uint32_t *cfgTexBufLen)  {
    *cfgTexBuf = (char*)malloc(*cfgTexBufLen);
    return 0;
}
void freeBuff(void *objPtr, uint32_t pid, const char *funcName, const int32_t fileLine){
    free(objPtr);
}

typedef struct tagDB_VNODE_RVKVS_CFG_S {
    uint32_t bgrThreadList;
} DB_VNODE_RVKVS_CFG_S;

uint32_t PmgrGetVnodeCfgFromMemDbByMode(DB_VNODE_RVKVS_CFG_S *rvKvsCfg)
{
    return rvKvsCfg->bgrThreadList;
}

int PmgrGetSpecGroupVnodeCfgInfo(uint32_t len)
{
    DB_VNODE_RVKVS_CFG_S *mem;
    uint32_t cfgTexBufLen = 123;

    allocArgBuff((char **)&mem, &cfgTexBufLen);
    int maxNum = PmgrGetVnodeCfgFromMemDbByMode(mem);
    freeBuff(mem, 123, "_function_name_", 976);
    return maxNum;
}

uint32_t PmgrGetRvThreadNumPerVnode(uint16_t groupId)
{
    PmgrGetSpecGroupVnodeCfgInfo(groupId); //FP should not be reported here
}
