class Foo {
public:
    int *field;
};

int main() {
    Foo foo11, foo22;
    foo22.field = 0;
    foo11.field = foo22.field;
    return *foo11.field; // should be detect here
}
