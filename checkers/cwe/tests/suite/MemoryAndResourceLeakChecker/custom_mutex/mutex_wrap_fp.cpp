
void se_lock_chip(int d);
void se_unlock_chip(int d);

void LockWrap(int d) {
    se_lock_chip(d);
}
void UnlockWrap(int d) {
    se_unlock_chip(d);
}
void foo() {
    int d = 1;
    LockWrap(d);
    UnlockWrap(d); // no MLC problem here
}
