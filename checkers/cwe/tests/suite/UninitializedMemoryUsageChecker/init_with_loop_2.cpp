int bar() { return 42; }

int foo()
{
    int a;
    while (1) {
        a = bar();
        break;
    }
    return a;
}
