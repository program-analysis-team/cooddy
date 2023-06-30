
void se_lock_chip1(int d);
void se_unlock_chip1(int d);
void foo() {
    int d = 3;
    se_lock_chip1(d);
    se_unlock_chip1(d);
    // no MLC problem here
}
