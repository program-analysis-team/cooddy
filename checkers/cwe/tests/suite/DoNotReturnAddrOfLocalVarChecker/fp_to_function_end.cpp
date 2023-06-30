struct A {
    int flag;
    int *x;
};

int aaa;   // 0.0.224.0

void foo(A *a) {   //a = 0.0.3.0; a->flag = 0.0.3.3
    int x = 5;          // 0.0.17.0
    //x = 5;
    if (a->flag) { // Condition 1
        a->x = &x;   // FP here
    }
    if (a->flag) { // Condition 2 == Condition 1
        a->x = nullptr; // Here local adress is removed, and no warning should be arraised
    }
}

void bar(){
    A* a = {};
    foo(a);
}
