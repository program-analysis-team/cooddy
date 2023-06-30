void func(int param, int** p)
{
    static int ret = 10;
    if (param == 10)
        *p = 0;
    else
        *p = &ret;
}

int main()
{
    int* a = 0;
    func(10, &a);
    return *a;
}
