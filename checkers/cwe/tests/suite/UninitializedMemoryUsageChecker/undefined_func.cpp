int local_memset(int* var);

int test()
{
    int var;
    local_memset(&var);
    return var + 1;
}
