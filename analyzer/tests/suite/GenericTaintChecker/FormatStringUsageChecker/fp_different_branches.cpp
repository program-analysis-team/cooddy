#include <stdio.h>

#include <fstream>
#include <string>

int main() {
    std::string val, fmt = "Hello";
    std::ifstream s("file.txt");
    s >> val;

    if (val == "10") {
        s >> fmt;
    } else {
        printf(fmt.c_str(), 10);
    }
    return 0;
}
