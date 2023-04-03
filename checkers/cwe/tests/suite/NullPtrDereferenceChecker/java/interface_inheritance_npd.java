interface I {
    class A {
    }
    default String bar() {
        return null;
    }
}

class B implements I {
}

class C extends B {
}

class Main {
    public static void main(String[] args) {
        C c = new C();
        String str = c.bar();
        str.length();
    }
}
