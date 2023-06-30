class A {
    void func(String[] fo111, String[] fo222) {
        fo111[1] = fo222[1];
    }

    int main() {
        String[] a = new String[10];
        String[] b = new String[5];
        b[1] = null;
        func(a, b);
        return a[1].length();
    }
}
