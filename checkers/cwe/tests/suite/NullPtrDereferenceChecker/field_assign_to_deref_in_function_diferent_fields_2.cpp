class Foo {
public:
    int *field;
    int *other;
};

void func(Foo *fo111, Foo *fo222) {
    fo111->field = fo222->other;
}

int main() {
    Foo f11;
    Foo f22;
    int a = 123;
    f22.field = 0;
    f22.other = &a;
    func(&f11, &f22);
    return *f11.field;
}
