int Square(int x, int y, bool add)
{
    int tmp = x * x;
    //  There is extra semicolon in the next line
    //  We simulate adding it by mistake
    if (add);
    tmp += y;

    return tmp;
}
