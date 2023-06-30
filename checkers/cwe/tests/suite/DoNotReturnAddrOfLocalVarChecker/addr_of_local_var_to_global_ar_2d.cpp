int* global[123][23];
void foo()
{
    int x = 0x42;
    global[1][1] = &x;
}
