

int g_var;

int foo()
{
    int p;
    if (g_var > 5) {
        p = 10;
    }
    if (g_var > 5) {
        return p;  // no error here
    }
    return 0;
}
