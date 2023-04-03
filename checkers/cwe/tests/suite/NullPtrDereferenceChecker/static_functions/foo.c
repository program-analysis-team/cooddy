
int* get_ptr();

int foo()
{
    int* p = get_ptr();
    return *p;  // NPD here
}
