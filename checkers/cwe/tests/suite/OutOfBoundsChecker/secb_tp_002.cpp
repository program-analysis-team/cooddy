#define MAX 100

void test_bad_02(char* p)
{
    int i;
    for (i = 0; i < MAX; i++) {
        /* POTENTIAL FLAW: Access buffer may be out of bounds */
        p[i] = i;  // error
    }
}
