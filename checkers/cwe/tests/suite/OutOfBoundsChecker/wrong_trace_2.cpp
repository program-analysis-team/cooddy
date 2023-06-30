
int someFunc(int param);

int getSize(int param)
{
    return someFunc(param);
}

void memAccess(char* mem)
{
    int idx = getSize(10);
    mem[idx];
}

void foo()
{
    char arr[10];
    memAccess(arr);
}
