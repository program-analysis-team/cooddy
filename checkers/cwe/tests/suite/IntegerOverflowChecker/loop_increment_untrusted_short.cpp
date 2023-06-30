
unsigned some();

void foo()
{
    auto s = some();
    for (short i = 0; i < s; i++) {  // error here
    }
}
