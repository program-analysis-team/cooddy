#include "iostream"
void memcpy(void* dst, void* src, int cnt);
void log(char* s) {
    printf("Password: %s", s);
}
char* foo(char* cc)
{
    SENSI_INFO char* usr = cc;

    SENSI_INFO char* pwd;
    if (cc == nullptr)
        pwd = "111";
    else
        pwd = cc;
    char pwd2[100];
    memcpy(pwd2, pwd, 100);
    log(pwd2);
    return usr;
}
