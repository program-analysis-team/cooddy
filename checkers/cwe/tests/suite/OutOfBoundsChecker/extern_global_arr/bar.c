extern int g_arr[];

void foo()
{
    g_arr[11];  // OOB error
}
