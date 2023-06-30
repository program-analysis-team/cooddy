#include <iostream>
#include <list>

void foo(std::list<int>& myList)
{
    size_t num;
    std::cin >> num;
    myList.resize(num);
}
