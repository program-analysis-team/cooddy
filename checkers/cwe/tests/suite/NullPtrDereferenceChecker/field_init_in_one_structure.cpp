struct Foo {
public:
    int *field;
    int *other;
};

int main() {
    Foo f11;
    f11.other = 0;
    f11.field = f11.other;
    return *f11.field; //should be detected here
}
