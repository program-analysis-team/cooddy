void test_bad_01(const char* pValue)
{
    char* pOut;
    const char* ptr;
    int ulLen = 0;
    ptr = pValue + 1;
    for (; *ptr; ptr++) {
        ptr++;
    }
}
