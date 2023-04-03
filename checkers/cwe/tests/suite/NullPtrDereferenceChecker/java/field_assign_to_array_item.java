class Foo {
    String field;
    String other;
}
class A {
    int main() {
        Foo f11 = new Foo();
        String[] f22 = new String[10];
        f22[1] = null;
        f11.field = f22[1];
        return f11.field.length(); // should be detected here
    }
}
