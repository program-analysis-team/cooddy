
#ifdef __AARCH64EB__

int g_arr[10];

void foo()
{
    g_arr[11];  // OOB error
}

#endif
