int* global[123];
void foo()
{
    int x = 0x42;
    global[1] = &x;
}
