
#include <string.h>

#include <iostream>

#define IVS_SUCCEED 1

int strncpy_s(char *dstBuff, int dstMax, const char *srcBuff, int srcSize);

bool SecurityDecrypt(const std::string &strTransID, std::string &inCipherString, char *outPlainBuff,
                     int &outPlainBuffLen)
{
    bool ret = true;
    char iv[32] = {0};
    int len = 32;

    std::string baseIV = inCipherString.substr(inCipherString.find("$") + 1);
    if (strncpy_s(iv, sizeof(iv), baseIV.c_str(), baseIV.size()) != IVS_SUCCEED) {  // FP here
        ret = false;
    }
    return ret;
}
