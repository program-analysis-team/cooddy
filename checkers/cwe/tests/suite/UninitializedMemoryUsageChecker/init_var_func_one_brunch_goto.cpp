void test(int ccc, int* ddd)
{
    if (ccc == 0) {
        goto label1;
    } else {
        *ddd = 4;
    }
    label1:
        return;
}

int main()
{
    int aaa;
    test(0, &aaa);
    int bbb = 12 + aaa;
    return bbb;
}
