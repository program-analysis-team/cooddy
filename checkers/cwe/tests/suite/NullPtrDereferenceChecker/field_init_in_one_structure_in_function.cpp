struct Foo {
public:
    int *field;
    int *other;
};

void func(Foo* fo111) {
    fo111->field = fo111->other;
}

int main() {
    Foo f11;
    f11.other = 0;
    func(&f11);
    return *f11.field; //should be detected here
}
