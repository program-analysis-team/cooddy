template <class T, class U>
struct Struct;

template <class T, class U = void>
class Class;

void f();
int f(int x);
int f(int y);
int f(double);
int& f(Struct<int*, double**>&& x);
int** f(Class<Struct<int*&, double>> x);
void f(const volatile int* const**&);

namespace ns {

void f();
int f(int x);
int f(double);

}  // namespace ns

void xf();
void fx();
