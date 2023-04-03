#include <stdint.h>

int32_t MSC_GetLocalSiteId(uint64_t clusterId, uint32_t *pLocalNodeId);

int32_t NasFmmGetLocalLsIDAdapter(uint64_t clusterId, uint32_t *pLocalNodeId)
{
    int32_t result;
    uint32_t workLsId;
    uint8_t localSiteId = 0;

    if (clusterId == 0) {
        localSiteId = 0;
    } else {
        result = MSC_GetLocalSiteId(clusterId, &localSiteId);
        if (result != 0) {
            do {
                localSiteId++;
            } while (0);
            Undef_GetId(pLocalNodeId);
            return 1;
        }
    }
    *pLocalNodeId = 20;
    return 0;
}

int func(uint64_t clusterId)
{
    uint32_t pLocalNodeId;
    NasFmmGetLocalLsIDAdapter(clusterId, &pLocalNodeId);
    return pLocalNodeId;  // no error here
}
