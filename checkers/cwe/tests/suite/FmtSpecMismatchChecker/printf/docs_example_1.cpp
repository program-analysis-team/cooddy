#include <algorithm>
#include <array>
#include <cstdio>
#include <string>
using namespace std;

int getIndex(const string& text)
{
    static array<string, 3> vegetables = {"cucumber", "potato", "squash"};

    auto it = find(vegetables.begin(), vegetables.end(), text);

    if (it == vegetables.end())
        return -1;  // Not found
    return it - vegetables.begin();
}

int main()
{
    // %u expects an unsigned integer, but if getIndex returns -1, an incorrect value will be printed.
    printf("%u", getIndex("tomato"));
}
