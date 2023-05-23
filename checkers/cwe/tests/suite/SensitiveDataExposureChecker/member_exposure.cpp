#include "iostream"
struct S {
    SENSI_INFO char* pwd;
};
void log(char* s) {
    printf("Password: %s", s);
}
void foo(S s)
{
    log(s.pwd);
}