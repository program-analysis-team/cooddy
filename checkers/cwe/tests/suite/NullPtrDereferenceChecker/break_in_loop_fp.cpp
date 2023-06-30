
int foo()
{
    static int v = 0;
    int* p = 0;
    unsigned int num = 0;
    for (auto i = 0; i < 10; i++) {
        if (num == 0) {
            p = &v;
            num++;
        } else if (*p == 0) {  // no NPD here
            num++;
            break;
        }
    }
    return num;
}
