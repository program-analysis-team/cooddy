enum class Colour { RED, GREEN, BLUE };

int Foo()
{
    int* p = 0;
    Colour x = Colour::BLUE;
    Colour y = Colour::BLUE;
    if (x == y) {
        return -1;
    }
    int b = *p;
    return 0;
}

int main()
{
    Foo();
}
