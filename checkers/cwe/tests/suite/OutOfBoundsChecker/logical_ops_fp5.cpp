
int g_arr[16];

#define b (256 / (1U << (10 - 6)))

void foo()
{
    for (unsigned int i = 0; i < b; ++i) {
        g_arr[i] = 10;  // no OOB here
    }
}
