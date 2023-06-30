int test(int ccc, int* ddd)
{
    if (ccc == 0) {
        return 0;
    } else {
        *ddd = 4;
        return 1;
    }
}

int main()
{
    int aaa;
    int rc = test(0, &aaa);
    if (rc) {
        int bbb = 12 + aaa;
        return bbb;
    } else {
        int bbb = 12;
        return bbb;
    }
}
