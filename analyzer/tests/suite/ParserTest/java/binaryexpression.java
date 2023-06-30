public class Main {
    public void foo(int x, int y, boolean bx, boolean by) {
        int i;
        i = x;                      // assign
        boolean b1 = x == y;        // equal
        boolean b2 = x != y;        // notequal
        boolean b4 = x >= y;        // ge
        boolean b6 = x <= y;        // le
        boolean b3 = x > y;         // gt
        boolean b5 = x < y;         // lt
        boolean b7 = bx && by;      // and
        boolean b8 = bx;
        b8 &= bx;                   // and_assign
        boolean b9 = bx & by;       // bitand
        boolean b10 = bx || by;     // or
        boolean b11 = bx;
        b11 |= by;                  // or_assign
        boolean b12 = bx | by;      // bitor
        boolean b13 = bx;
        b13 ^= by;                  //xor_assign
        boolean b14 = bx^by;        // xor
        int i2 = x / y;             // div
        i /= x;                     // div_assign
        int i3 = x * y;             // mul
        i *= x;                     // mul_assign
        int i4 = x + y;             // add
        i += x;                     // add_assign
        int i5 = x - y;             // sub
        i -= x;                     // sub_assign
        int i6 = x % y;             // mod
        i %= x;                     // mod_assign
        x <<= y;                    // lshift_assign
        x >>= y;                    // rshift_assign
        int i7 = x << y;            // lshift
        int i8 = x >> y;            // rshift
    }
}
