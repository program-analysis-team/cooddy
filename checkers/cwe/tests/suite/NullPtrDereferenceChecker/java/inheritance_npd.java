class A {
    String bar() {
        return null;
    }
}

class B extends A {
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
