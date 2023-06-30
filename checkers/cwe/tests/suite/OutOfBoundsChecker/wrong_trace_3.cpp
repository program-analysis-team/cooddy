
int someFunc(int param, int* res);

void getSize(int param, int* res)
{
    int ret = 0;
    someFunc(param, &ret);
    *res = ret;
}

void memAccess(char* mem)
{
    int idx = 0;
    getSize(10, &idx);
    mem[idx];
}

void foo()
{
    char arr[10];
    memAccess(arr);
}
