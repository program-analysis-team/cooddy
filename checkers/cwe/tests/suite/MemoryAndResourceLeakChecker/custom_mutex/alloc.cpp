void se_lock_chip1(int d);
void se_unlock_chip1(int d);
int a();

void foo() {
    int d = 3;
    se_lock_chip1(d);
    if(a()) {
        return;
    }
    se_unlock_chip1(d);
    // no MLC problem here
}
