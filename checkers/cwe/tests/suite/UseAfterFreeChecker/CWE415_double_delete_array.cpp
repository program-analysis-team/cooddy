
void foo()
{
    int* a = new int[100];
    delete [] a;
    delete [] a;
}
