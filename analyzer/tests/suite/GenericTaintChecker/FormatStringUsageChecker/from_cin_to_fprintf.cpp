#include <stdio.h>

#include <iostream>
#include <string>

int main() {
    std::string hello = "Hello", world;
    std::cin >> world;

    hello += world;

    FILE *f = fopen("test.txt", "w");
    if (f != nullptr) {
        fprintf(f, hello.c_str(), " world");
    }
    return 0;
}
