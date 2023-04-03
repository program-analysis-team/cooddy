#include <stdint.h>
#include <string.h>
#define BYTE_PER_SECTOR_NOPI 200
int memcpy_s(void* dstBuff, int dstMax, void* srcBuff, int srcSize);

struct SGL_S {
    struct {
        char* buf = 0;
    } entrys[100];
};

int32_t copyCompressData(void* dst, uint16_t offset, uint32_t size, SGL_S* data, uint32_t entryIndex,
                         uint32_t curPageOffset)
{
    if (offset + size <= BYTE_PER_SECTOR_NOPI) {
        memcpy_s(dst, BYTE_PER_SECTOR_NOPI, data->entrys[entryIndex].buf + curPageOffset + offset, size);
    }
    return 0;
}

int32_t CompCheckGetStrongHashData(SGL_S* data, uint32_t* dataOffset, uint32_t* dataLen, uint32_t* totalLen,
                                   uint32_t entryIndex, uint32_t curPageOffset)
{
    char headerStrongHash[200];
    return copyCompressData(headerStrongHash, 0, sizeof(headerStrongHash), data, entryIndex, curPageOffset);
}
