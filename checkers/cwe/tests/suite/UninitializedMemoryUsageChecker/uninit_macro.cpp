#define FOO2(px0, px1) *px1

int foo()
{
    int qqq;
    int xxx;
    return FOO2(qqq, &xxx);
}
