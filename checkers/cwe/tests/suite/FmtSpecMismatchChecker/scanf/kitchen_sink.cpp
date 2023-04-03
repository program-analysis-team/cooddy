#include <cstdint>
#include <cstdio>
#include <vector>

int main()
{
    signed char ichar;
    short short_;
    int int_;
    long long_;
    long long llong_;
    std::ptrdiff_t isize;

    unsigned char uchar;
    unsigned short ushort;
    unsigned int uint;
    unsigned long ulong;
    unsigned long long ullong;
    std::size_t usize;

    scanf("%% %*d");

    scanf("%hhd %hd %d %ld %lld %jd", &ichar, &short_, &int_, &long_, &llong_, &isize);
    scanf("%hhd %hd %d %ld %lld %jd", &uchar, &ushort, &uint, &ulong, &ullong, &usize);
    scanf("%zd %td", &usize, &isize);

    char c;
    wchar_t wc;

    scanf("%c  %s  %[123]  %[0-9]  %[^123]  %[]123]  %[^]123] ", &c, &c, &c, &c, &c, &c, &c);
    scanf("%lc %ls %l[123] %l[0-9] %l[^123] %l[]123] %l[^]123]", &wc, &wc, &wc, &wc, &wc, &wc, &wc);

    float f32;
    double f64;
    long double f_longest;

    scanf("%f %lf %Lf", &f32, &f64, &f_longest);

    int* pa;
    double* pb;
    size_t* pc;
    void* pd;
    std::vector<int>* pe;

    scanf("%p %p %p %p %p", &pa, &pb, &pc, &pd, &pe);
}
