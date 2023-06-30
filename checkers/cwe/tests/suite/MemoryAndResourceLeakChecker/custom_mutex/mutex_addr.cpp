
void se_lock_chip(int& d);
void se_unlock_chip(int& d);

void foo()
{
    int d = 1;
    se_lock_chip(d);
    se_unlock_chip(d);  // no MLC problem here
}
