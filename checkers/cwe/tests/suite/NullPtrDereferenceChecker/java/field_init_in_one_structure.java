class Foo {
    String field;
    String other;
}
class A {
    int main() {
        Foo f11 = new Foo();
        f11.other = null;
        f11.field = f11.other;
        return f11.field.length(); //should be detected here
    }
}
