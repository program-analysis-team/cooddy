
int* foo();

int goo()
{
    int* p = foo();
    return *p;
}
