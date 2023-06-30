void test(int ccc, int* ddd)
{
    if (ccc == 0) {
        ccc = 1;
    } else {
        *ddd = 4;
    }
}

int main()
{
    int aaa;
    test(0, &aaa);
    int bbb = 12 + aaa;
    return bbb;
}
