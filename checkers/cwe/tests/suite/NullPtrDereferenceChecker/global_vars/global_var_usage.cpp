unsigned int g_shareL2MaxMacUserNum = 20;
char g_usr;
static __inline__ char* COM_GetdMacUserInstance(unsigned int userId)
{
    if (userId >= g_shareL2MaxMacUserNum) {
        return 0;
    }
    return &g_usr;
}
int MM_MubfIsDataCleanMetRmvCondition(unsigned int macId)
{
    char* userInst = 0;
    if (macId >= g_shareL2MaxMacUserNum) {
        return 0;
    }
    userInst = COM_GetdMacUserInstance(macId);
    *userInst;
}
