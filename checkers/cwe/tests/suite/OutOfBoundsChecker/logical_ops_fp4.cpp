
int g_arr[2];

void foo(int val)
{
    char b = (val == 10 ? 0 : 1);
    b ^= 1;
    g_arr[b] = 10;  // no OOB here
}
