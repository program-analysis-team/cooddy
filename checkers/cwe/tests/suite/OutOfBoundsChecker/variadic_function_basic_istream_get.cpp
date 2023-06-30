#include <iostream>
#include <sstream>

void foo()
{
    char ch;
    char buf;
    std::stringstream ss;
    long long index = std::cin.get();
    int arr[14];
    arr[index] = 987;
    std::cin.get(ch);
    arr[ch] = 5678;
    std::cin.get(&buf, 12);
    arr[buf] = 1;
    std::cin.get(&buf, 12, ' ');
    arr[buf] = 1;
    std::cin.get(*ss.rdbuf());
    arr[ss.get()] = 1;
}
