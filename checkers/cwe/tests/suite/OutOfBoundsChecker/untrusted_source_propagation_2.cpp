#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK foo(unsigned int idx, int idx_val)
{
    if (idx < 0 || idx >= 10) {
        return 0;
    }
    int idx_arr[10] = {0};
    int buff[10] = {0};

    idx_arr[idx] = idx_val;
    int v = idx_arr[idx];
    return buff[v];
}
