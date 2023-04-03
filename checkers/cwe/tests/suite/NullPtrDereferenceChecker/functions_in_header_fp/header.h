
static inline void get_ptr(int** ptr)
{
    static int var = 0;
    *ptr = &var;
}
