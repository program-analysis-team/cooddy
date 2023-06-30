void func(int** p)
{
    static int ret = 10;
    *p = &ret;
}

int main()
{
    int* a = 0;
    func(&a);
    return *a;
}
