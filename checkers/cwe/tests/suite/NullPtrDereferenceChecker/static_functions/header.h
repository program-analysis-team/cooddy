
static inline int* get_ptr()
{
    static int var = 0;
    return &var;
}
