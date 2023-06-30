class IntTest {
    int foo_oct(int x) {
        int y = 052 - 42;
        return x / y;
    }
    int foo_hex(int x) {
        int y = 0x2A - 42;
        return x / y;
    }
    int foo_binary(int x) {
        int y = 0b00101010 - 42;
        return x / y;
    }
    int foo_long(int x) {
        int y = 42L - 42;
        return x / y;
    }
}

class FloatTest {
    int foo(int x) {
        float f1 = 42.0f;
        double f2 = 42.0;
        return 0;
    }
}
