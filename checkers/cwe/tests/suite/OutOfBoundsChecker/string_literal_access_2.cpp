void foo1()
{
    char dest[16] = {0};
    char tmp[32] = "0123456789012345678901234567890";
    unsigned size = sizeof(tmp);
    unsigned index = size - 1;
    dest[index] = tmp[index];
}

void foo2()
{
    char tmp[32] = "0123456789012345678901234567890";
    unsigned size = sizeof(tmp);
    unsigned index = size - 1;
    char c = tmp[index];
}
