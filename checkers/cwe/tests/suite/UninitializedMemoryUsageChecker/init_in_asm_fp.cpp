
static inline long long NasAtomicCmpxchg(long *counter, long curValue, long nextValue)
{
    long long ret;
    __asm__ __volatile__("lock; cmpxchgq %2,%1"
                         : "=a"(ret), "+m"(*(volatile long *)counter)
                         : "r"(nextValue), "0"(curValue)
                         : "memory");
    return ret;
}
