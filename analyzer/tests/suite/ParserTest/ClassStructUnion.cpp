namespace A {
// Forward declaration
class MyName1;

class MyName1 {
public:
    MyName1();

    MyName1(const MyName1&);
    MyName1(MyName1&&);

    MyName1& operator=(const MyName1&);
    MyName1& operator=(MyName1&&);

    ~MyName1();
};

struct MyName2 {};
}  //  namespace A

namespace B {

struct MyName1 {};

union MyName2 {
    int myIntValue;
    double myDoubleValue;
};

class MyName3 {
private:
    A::MyName1 myValue;
};

}  //  namespace B
