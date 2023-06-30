
void foo(unsigned char c)
{
    int v[256];
    for (unsigned char i = 1; i <= c; i++) {
        v[i];  // FP here
    }
}
