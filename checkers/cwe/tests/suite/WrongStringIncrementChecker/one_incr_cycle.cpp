int sqlite3StrICmp(const char* zLeft, const char* zRight)
{
    unsigned char* a;
    unsigned char* b;
    a = (unsigned char*)zLeft;
    b = (unsigned char*)zRight;
    for (;;) {
        if (*a == '5' || *b == '6') {
        } else {
            break;
        }
        a++;
        b++;
    }
    return 123;
}
