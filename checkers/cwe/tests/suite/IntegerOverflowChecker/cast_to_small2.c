void CWE190_int_overflow_case_error_10()
{
    int a = 0x4000;
    int b = 0x4000;
    short len = (a + b);
}
void FP(int a, short b)
{
    short len = a + b;  // FP here
}
void FP2(int a, int b)
{
    short len = a + (b >> 2);  // FP here
}

void FP3(int a, int b)
{
    short len = a >> b;  // FP here
}

void FP4(int a)
{
    short len = a % 1024;  // FP here
}

void FP6(int a)
{
    short len = 123 + (a % 1024);  // FP here
}
