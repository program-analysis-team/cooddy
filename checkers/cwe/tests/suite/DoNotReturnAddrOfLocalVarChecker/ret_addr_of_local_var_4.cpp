struct FILE_INFO_T {
    int x;
};

struct NasMandatoryCheckArgs {
    FILE_INFO_T *pFileInfo;
};

struct DELEGATION_OP_ATTRS_T {
    NasMandatoryCheckArgs mandatoryLockCheckReq;
};

int processMandatoryLockForRemove(DELEGATION_OP_ATTRS_T *stDelegAttrs)
{
    int gfpStatus = 0;
    FILE_INFO_T file_info_with_uuid = {0};
    stDelegAttrs->mandatoryLockCheckReq.pFileInfo = &file_info_with_uuid;
    return gfpStatus;
}

int fssCallbackRemoveFast(DELEGATION_OP_ATTRS_T *stDelegAttrs, int flag)
{
    int gfpStatus = 0;
    if (flag == 0) {
        gfpStatus = processMandatoryLockForRemove(stDelegAttrs);
    }
    return gfpStatus;
}
