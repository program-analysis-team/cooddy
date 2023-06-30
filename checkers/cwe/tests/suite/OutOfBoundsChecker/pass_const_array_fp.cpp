
void access2(char* buff)
{
    buff[2] = 0;
}

void access8(char* buff)
{
    buff[8] = 0;
}

void foo(bool flag)
{
    char buff[6] = {0};
    if (flag)
        access8(buff);
    access2(buff);
}
