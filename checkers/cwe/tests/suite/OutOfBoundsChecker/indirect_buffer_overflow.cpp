
void foo()
{
    char str1[30], str2[18];
    char *p1 = str1, *p2 = str2;
    for (int i = 0; i < 10; i++, p1 += 3, p2 += 2) {
        *p1 = 0;  // no error here
        *p2 = 0;  // error here
    }
}
