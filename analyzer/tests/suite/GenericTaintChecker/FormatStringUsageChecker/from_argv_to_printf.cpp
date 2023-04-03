#include <stdio.h>
#include <string>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK main(int argc, char** argv)
{
    std::string fmt = "format string ";
    std::string s = argv[1];
    s += " 123 ";
    s += fmt;
    printf((fmt + s + " end").c_str());
}
