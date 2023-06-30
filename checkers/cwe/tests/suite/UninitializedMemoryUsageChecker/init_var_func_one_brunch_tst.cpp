//typedef unsigned char u8;

void test(int ccc, int& ddd)
{
    if (ccc == 0) {
        ccc = 1;
    } else {
        ddd = 4;
    }
}

int main()
{
    int var;
    int b = 0;
    int* d = &b;
    test(*d, var);
    return 1;
}
