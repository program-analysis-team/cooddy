#include <stdint.h>
#include <stdlib.h>

struct Opt {
    char* s;
};

uint32_t GetOpt(char** s, Opt* opt);

void GetZoneTypeAndDpId(int32_t argc, char* argv[], uint32_t* type, uint32_t* dpId)
{
    Opt opt;
    *type = 0;
    GetOpt(&argv[1], &opt);
    *dpId = atoi(opt.s);
}

void showRecAndWriteSize(int32_t argc, char* argv[])
{
    char arr[10];
    uint32_t type;
    uint32_t dpId;
    GetZoneTypeAndDpId(argc, argv, &type, &dpId);
    arr[dpId] = 10;
}
