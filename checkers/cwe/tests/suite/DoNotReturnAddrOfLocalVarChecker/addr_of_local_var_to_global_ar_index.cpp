int* global[123];
void foo(int i)
{
    int x = 0x42;
    global[i] = &x;
}
