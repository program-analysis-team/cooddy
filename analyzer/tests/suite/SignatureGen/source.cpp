template <class T, class U>
struct Struct;

template <class T, class U = void>
class Class;

void f1();
int f2(int x);
int f3(double);
int& f4(Struct<int*, double**>&& x);
int** f5(Class<Struct<int*&, double>> x);
void f6(const int volatile const* const**&);
void f7(volatile const int);

namespace ns {

void f8();

}

class X {
public:
    void f9();

protected:
    void f10();

private:
    void f11();
};

using myInt = int;
myInt f12();

#define MACRO_RETURN Struct<int, double>
MACRO_RETURN f13();
