extern int g_arr[];
extern int get_idx();

void bar()
{
    // error on the next line
    g_arr[get_idx()];
}
