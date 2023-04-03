
void test_good_02(unsigned char* p, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        *(p + i) = i;  // no error here
    }
}
