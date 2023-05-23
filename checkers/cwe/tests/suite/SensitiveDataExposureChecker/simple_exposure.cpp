#include "iostream"
void log(char* s) {
    printf("Password: %s", s);
}
void foo()
{
    SENSI_INFO char* pwd = "12345";
    log(pwd);
}