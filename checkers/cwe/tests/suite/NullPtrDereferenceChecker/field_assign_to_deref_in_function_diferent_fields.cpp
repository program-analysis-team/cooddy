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
    f22.other = 0;
    func(&f11, &f22);
    return *f11.field; // should be detect here
}
