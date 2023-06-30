#include <cstdio>

#define SUSRCSIRS_CSIRSCFG_TAG_NZPRES_INPUT_NULL_PTR 1
#define SUSRCSIRS_CSIRSCFG_TAG_USRID_INVALID 2
#define L2_OK 0
typedef unsigned int UINT32;

#define IS_PTR_INVALID(ptr) ((ptr) == NULL)
#define IS_USRID_INVALID(usrId) ((usrId) < 0 || (usrId) > 100)

#define SUSRCSIRS_TAG_DATAVALID_CHECKFUNC(usrId, data, tagName) ({    \
    UINT32 macroRet = L2_OK;                                          \
    if (IS_PTR_INVALID(data)) {                                       \
        macroRet = SUSRCSIRS_CSIRSCFG_TAG_##tagName##_INPUT_NULL_PTR; \
    }                                                                 \
    if (IS_USRID_INVALID(usrId)) {                                    \
        macroRet = SUSRCSIRS_CSIRSCFG_TAG_USRID_INVALID;              \
    }                                                                 \
    macroRet;                                                         \
})


unsigned int ProcNzpResTagCheck(unsigned short usrId, void *data, unsigned int dataLen)
{
    unsigned int ret = SUSRCSIRS_TAG_DATAVALID_CHECKFUNC(usrId, data, NZPRES);
    if (ret != L2_OK) {
        return ret;
    }
    char *tag = (char *)data;
    *tag = 'a';
    return L2_OK;
}
