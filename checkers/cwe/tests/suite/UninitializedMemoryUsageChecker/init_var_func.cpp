void test(int& a)
{
    a = 4;
}

int main()
{
    int a;
    test(a);
    return 2 + a;
}
