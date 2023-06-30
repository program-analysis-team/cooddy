class Foo {
public:
    int *field;
    int *other;
};

void func(Foo* fo111, int* fo222[10]) {
    fo111->field = fo222[1];
}

int main() {
    Foo f11;
    int* f22[10];
    f22[1] = 0;
    func(&f11, f22);
    return *f11.field; // should be detected here
}
