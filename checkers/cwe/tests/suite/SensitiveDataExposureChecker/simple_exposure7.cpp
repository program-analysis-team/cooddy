#include "iostream"
#include "string"
void log(std::string s) {
    printf("Password: %s", s.c_str());
}
void foo(SENSI_INFO std::string cc)
{
    std::string pwd;
    if (cc.size() == 0)
        return;
    else
        pwd = cc;
    log(pwd);
}