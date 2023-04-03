void foo()
{
    char arr[100][200];
    arr[99][300];  // Detect here
}

void foo2()
{
    char arr[100][200][300];
    arr[99][300][200];  // Detect here
}

void foo3()
{
    char arr[100][200][300];
    arr[99][100][400];  // Detect here
}

void memcpy(void*, void*, int);

void foo4()
{
    char arr[100][200];
    memcpy(&arr[0][0], &arr[0][0], 400);  // FP
}

int memcpy_s(void*, int, void*, int);
struct A {
    char arr[100][200];
};
void foo5(A* a, A* c)
{
    A* b = c;
    memcpy_s(&b->arr[0][0], sizeof(b->arr), &a->arr[0][0], sizeof(a->arr));  // FP
}

void foo6()
{
    unsigned long long arr[2][2];
    arr[1][0];
    arr[0][1] |= 1;
    arr[1][1];  // FP due to arr[0] is replaced to *arr in symbolic execution.
}

struct S {
    double thirdFour[4][4][4];
    double third2V[4][4][4];
    double l2V_vals[4][4][4];
};

void FillLValues(S s)
{
    for (unsigned k = 0; k < 4; ++k) {
        s.l2V_vals[0][0][k] += s.third2V[0][0][k];
        s.thirdFour[0][0][k];  // FP here due to arr[0] is replaced to *arr in symbolic execution.
    }
}
