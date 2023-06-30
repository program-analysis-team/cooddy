// Source: class output parameter
// Propagation: transformation function
// Path sensitivity: condition in loop
// Sink: pointer arithmetic
#include <charconv>
#include <string>

class X {
public:
    int a[10];
    bool b;
    std::string c;
};

// Unlike 6b, this example does not mutate val and does not introduce taint.
void taint(const X& val);

void f(bool cond)
{
    int arr[10] = {};

    X val;
    taint(val);

    int x = 0;
    std::from_chars(&val.c[0], &val.c[0] + val.c.size(), x);

    for (int i = 0; cond and i < 5; i++) {
        *(arr + x) = 0;
    }
}
