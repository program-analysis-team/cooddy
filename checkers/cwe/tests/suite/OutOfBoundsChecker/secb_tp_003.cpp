
void test_bad_03(unsigned char* p)
{
    int i;
    for (i = 0; i < 100; i++) {
        /* POTENTIAL FLAW: Access buffer may be out of bounds */
        *(p + i) = i;  // error
    }
}
