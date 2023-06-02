#include <cstddef>

// no type named 'align_val_t' in namespace 'std'
auto not_declared_align_val_t()
{
    // gcc declares align_val_t without any system headers
    std::align_val_t a;
    return a;
}

#include <algorithm>
#include <string>

// -fdelayed-template-parsing
struct SVec {
    std::string v;
};

template <typename T>
class S {
    void foo(T& p, std::string& v)
    {
        SVec svec;
        v = svec;  // there is no overloaded operator
    }
};

// -Wno-non-pod-varargs
void pod_varargs()
{
    std::string s;
    printf("%s", s);
}

// -Wno-invalid-constexpr
constexpr float invalid_constexpr(float f)
{
    return __builtin_acosf(f);
}

// -Wno-c++11-narrowing
void narrowing(unsigned int p1, unsigned int p2)
{
    int a{p1 - p2};
}

// 'decltype(auto)' cannot be combined with other type specifiers
const decltype(auto) alone_decltype(int n)
{
    return n;
}

// initializing wide char array with incompatible wide string literal
void alone_decltype()
{
    char16_t arr[] = L"ads";
}

// variable-sized object may not be initialized
char varsize_init(int n)
{
    char arr[n] = "ads";
    return arr[0];
}

// in capture list does not name a variable
// reference to local binding
void capture_bindings(std::pair<char, char> p)
{
    auto [l, f] = p;
    std::string s;
    // clang doesn't support bindings in lambdas
    std::sort(s.begin(), s.end(), [l](char c1, char c2) { return c1 < l; });
}
