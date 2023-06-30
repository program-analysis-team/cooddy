
static const unsigned int MAX_RAND_NUM = 10;

extern void get_random_bytes(void *buf, int nbytes);

unsigned int g_randArrays[MAX_RAND_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

unsigned int getRandGp()
{
    unsigned int randNum = 0;
    get_random_bytes((void *)&randNum, 4);
    return g_randArrays[randNum % MAX_RAND_NUM];
}
