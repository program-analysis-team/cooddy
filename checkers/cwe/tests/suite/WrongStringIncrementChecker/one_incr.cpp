void test_bad_01(const char* pValue)
{
    char *pOut;
    const char* ptr;
    int ulLen = 0;
    ptr = pValue + 1;
    while ((*ptr != '\"') && *ptr) {
        ulLen++;
        if (*ptr++ == '\\') {
            bool val = *ptr + 1;
        }
    }
}
