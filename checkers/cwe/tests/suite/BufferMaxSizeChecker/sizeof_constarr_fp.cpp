int memcpy_s(void* dstBuff, int dstMax, const void* srcBuff, int srcSize);

#define MAX_SIZE 48

typedef struct B {
    float a;
} STASH_B;

typedef struct A {
    STASH_B* sfpPrefix1[6];
    STASH_B* sfpPrefix[MAX_SIZE];
    STASH_B* sfpPrefix2[12];
} STASH_OP_KEY_S;

void foo(STASH_OP_KEY_S* dstBuff)
{
    memcpy_s(dstBuff, sizeof(dstBuff->sfpPrefix), 0, MAX_SIZE * sizeof(STASH_B*));
}
