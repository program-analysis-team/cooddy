#include <cstdint>
#include <cstdio>
#include <vector>

int scanf_s(const char* _Format, ...);

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

    scanf_s("%% %*d");

    scanf_s("%hhd %hd %d %ld %lld %jd", &ichar, &short_, &int_, &long_, &llong_, &isize);
    scanf_s("%hhd %hd %d %ld %lld %jd", &uchar, &ushort, &uint, &ulong, &ullong, &usize);
    scanf_s("%zd %td", &usize, &isize);

    char c;
    wchar_t wc;

    scanf_s("%c", &c, 1);
    scanf_s("%s", &c, 1);
    scanf_s("%[123]", &c, 1);
    scanf_s("%[0-9]", &c, 1);
    scanf_s("%[^123]", &c, 1);
    scanf_s("%[]123]", &c, 1);
    scanf_s("%[^]123]", &c, 1);

    scanf_s("%lc", &wc, 1);
    scanf_s("%ls", &wc, 1);
    scanf_s("%l[123]", &wc, 1);
    scanf_s("%l[0-9]", &wc, 1);
    scanf_s("%l[^123]", &wc, 1);
    scanf_s("%l[]123]", &wc, 1);
    scanf_s("%l[^]123]", &wc, 1);

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
