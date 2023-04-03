void foo(unsigned int *ptr){};

int main()
{
    unsigned short *buffer = 0;
    foo((unsigned int *)buffer);
    return 0;
}
