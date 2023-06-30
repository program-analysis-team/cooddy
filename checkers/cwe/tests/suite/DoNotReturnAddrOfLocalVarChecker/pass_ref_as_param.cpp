typedef struct {
    volatile long counter;
} atomic_t;

static inline int atomic_read(const atomic_t *v)
{
    return v->counter;
}

int gfpDcacheBeWriteSingle()
{
    int ret = 0;
    atomic_t lwtRetVal;
    if (atomic_read(&lwtRetVal) != 0) {
        ret = (int)atomic_read(&lwtRetVal);
    }
    return ret;
}
