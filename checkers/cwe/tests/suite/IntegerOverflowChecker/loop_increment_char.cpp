
void foo()
{
    int s1 = 100;
    for (char i = 0; i < s1; i++) {  // no error here
    }
    int s2 = 1000000;
    for (char i = 0; i < s2; i++) {  // error here
    }
}
