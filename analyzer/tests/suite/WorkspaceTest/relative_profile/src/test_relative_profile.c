#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#define NULL 0

#define RETURN_OK 0
#define RETURN_ERR 1

void* MSG_AllocMsgSub(unsigned int v_pMsgLength, const char *v_PFunction, unsigned int vuiLine);
void* MSG_FreeMsgSub(void* pMsg);

#define Msg_Alloc(uiLength) MSG_AllocMsgSub(uiLength, __FUNCTION__, __LINE__)
#define Msg_Free(pMsg) MSG_FreeMsgSub(pMsg)

int test_check_func(void* pValue) {
    return RETURN_ERR;
}

void test_case_02() {
    int iRet;
    int tmpMsgLen = 1;
    void* tmpMsg = NULL;

    tmpMsg = Msg_Alloc(tmpMsgLen);

    iRet = test_check_func(tmpMsg);

    if(unlikely(iRet != RETURN_OK)) {
        /*MSG_FreeMsg(tmpMsg);*/
        return;
    }
    memset(tmpMsg, 0, tmpMsgLen);
}

void test_case_02_test() {
    int tmpMsgLen = 0;
    void* tmpMsg = NULL;

    /*tmpMsg = MSG_AllocMsgSub(tmpMsgLen,__FUNCTION__,__LINE__);*/
    tmpMsg = malloc(tmpMsgLen);
}

void main(){
    test_case_02();
    test_case_02_test();
}
