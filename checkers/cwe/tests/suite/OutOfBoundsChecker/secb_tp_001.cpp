#define MAX 100

void test_bad_01(char* p, char* q)
{
    char *tmp, *tmp1;
    tmp = q;
    tmp1 = p;
    int i;
    for (i = 0; i < MAX; i++) {
        /* POTENTIAL FLAW: Access buffer may be out of bounds */
        tmp[i] = i;  // error
    }
}
