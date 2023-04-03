
void foo()
{
    char str1[10], str2[18];
    char *p1 = str1, *p2 = str2 + 16;
    int i = 0;
    while (i < 10) {
        *p1++ = 0;  // no error here
        *p2 = 0;    // error here
        p2 -= 2;
        i++;
    }
}
