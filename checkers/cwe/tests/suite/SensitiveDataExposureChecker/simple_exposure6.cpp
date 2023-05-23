#include "iostream"
#include "string"
SENSI_INFO std::string pwd;
void foo(std::string v)
{
    pwd = v;
    std::string val;
    val = v + pwd;
    printf("Password: %s", val.c_str());
}