#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

// TODO: there are lot of overloaded functions "replace"
//       this test case has to be extended during implementation
//       and all functions has to be covered
int main(int argc, char* argv[]) {
    std::string buf, fmt = "";
    std::cin >> buf;
    fmt = buf.replace(buf.find('D'),1,"d");
    printf(fmt.c_str(), "Hello!");
    return 0;
}
