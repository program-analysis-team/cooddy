
int test()
{
    int arr[10] = {0};
    int idx = 0;
    idx /= 5;
    idx >>= 2;
    idx <<= 2;
    idx &= 5;
    idx |= 6;
    idx = (idx | 4) ^ idx;
    idx = + idx;
    idx = -idx;
    return arr[idx - 1];
}
