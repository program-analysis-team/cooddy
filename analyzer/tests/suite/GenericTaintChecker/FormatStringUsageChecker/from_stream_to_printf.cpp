#include <stdio.h>

#include <fstream>
#include <string>

int main() {
    std::ifstream s("file.txt");
    std::string val, fmt;
    s >> val >> fmt;

    if (val == "10") {
        return 0;
    }
    printf(fmt.c_str(), 10);
    return 0;
}
