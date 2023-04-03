
void foo(unsigned int v)
{
    int arr[0xFFFF];
    arr[(v >> 16) & 0xFFFFU] = 0;  // no error
}
