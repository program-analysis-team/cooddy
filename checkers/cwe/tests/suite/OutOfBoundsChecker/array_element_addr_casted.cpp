#include <memory>

#define TLM_MAC_ADDR_LEN 6
#define VOS_UINT32 uint32_t
#define VOS_UINT8 uint8_t

VOS_UINT32 foo()
{
    const VOS_UINT8 aucDstMac[TLM_MAC_ADDR_LEN] = {0};
    auto ret = (reinterpret_cast<const VOS_UINT32*>(&aucDstMac[4]));
    return *ret;
}
