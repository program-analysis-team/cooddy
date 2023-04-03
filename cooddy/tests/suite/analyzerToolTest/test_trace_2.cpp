void initRet(int* ret, int flag)
{
    if (!ret) {
        return;
    }
    if (flag == 0xAA) {
        *ret = 10;
    }
}

int foo(int flag1, int flag2)
{
    if (flag1 == 0xBB) {
        return 0;
    }

    int ret;
    initRet(&ret, flag2);
    return ret;
}
