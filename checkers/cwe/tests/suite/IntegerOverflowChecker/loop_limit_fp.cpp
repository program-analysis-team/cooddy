
int foo(short v)
{
    int ret = 0;
    for (short i = 0; i < v; ++i) {  // no error here
        ret++;
    }
    return ret;
}
