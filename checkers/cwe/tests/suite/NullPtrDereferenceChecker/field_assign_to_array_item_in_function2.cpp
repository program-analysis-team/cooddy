class Foo {
public:
    int *field;
    int *other;
};

void func(Foo* fo111, int* fo222[10]) {
    fo222[1] = fo111->field;
}

int main() {
    Foo f11;
    int* f22[10];
    f11.field = 0;
    func(&f11, f22);
    return *f22[1]; // should be detected here
}
