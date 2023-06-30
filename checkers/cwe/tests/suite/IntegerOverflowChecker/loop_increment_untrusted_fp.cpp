
unsigned some();

void foo(short* s)
{
    *s = some();
    for (short i = 0; i < *s; i++) {  // no error here
    }
}
