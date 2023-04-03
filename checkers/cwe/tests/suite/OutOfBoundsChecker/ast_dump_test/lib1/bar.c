extern int g_arr[];
extern int get_idx();

void bar()
{
    g_arr[get_idx()];  // no error here
}
