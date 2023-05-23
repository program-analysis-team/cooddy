#include "iostream"
#include "string"
void log(const char* s) {
    printf("Password: %s", s);
}
void foo(SENSI_INFO char* cc)
{
    const char* pwd;
    if (cc == 0)
        return;
    else
        pwd = cc;
    log(pwd);
}