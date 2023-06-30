#define MAX 100

void test_bad_05(char* p)
{
    int i;
    char buff[MAX];
    for (i = 0; i < MAX; i++) {
        /* POTENTIAL FLAW: Access buffer may be out of bounds */
        buff[i++] = *p++;  // error
    }
}
