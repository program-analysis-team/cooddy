#include <cstdio>
#include <string>
using namespace std;

struct EmployeeInfo {
    string name;
    int age;
    char favorite_letter;

    int dumpToFile(FILE* f)
    {
        // Due to a different amount of format specifiers and passed values, important information is not printed.
        fprintf(f, "%s, %d", name.c_str(), age, favorite_letter);
    }
};
