#include <stdint.h>

struct Bucket {
    char prefix[10];
};

void trySearchInPos(int32_t pos, Bucket *bucket)
{
    bucket->prefix[pos];
}

void searchNodeInFastTable(Bucket *bucket, int32_t pos)
{
    trySearchInPos(pos, bucket);

    uint32_t bitmap = bitmap ^= (1U << pos);
    while (bitmap != 0) {
        int32_t index = __builtin_ffs((int)bitmap);
        pos = index - 1;
        trySearchInPos(pos, bucket);
    }
}
