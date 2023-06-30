class Foo {
public:
    int *field;
    int *other;
};

int main() {
    Foo f11;
    int* f22[10];
    f22[1] = 0;
    f11.field = f22[1];
    return *f11.field; // should be detected here
}
