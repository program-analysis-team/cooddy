#include <chrono>
#include <iostream>
#include <list>
#include <vector>

template <class T>
//  suppressed checker for reserving size of containers as this is template method
//  COODDY_SUPPRESS ContainerSpecificationChecker
double Test(T &container, std::size_t n = 10000000)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i <= n; ++i) {
        container.push_back(i);
    }

    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
}

//  COODDY_SUPPRESS
int main()
{
    std::vector<size_t> a;
    std::list<size_t> b;

    double timeA;
    // COODDY_SUPPRESS        LocalVariableInitialValueChecker UnusedVariablesChecker
    double timeB;

    timeA = Test(a);
    timeB = Test(b);

    std::cout << "Vector: " << timeA << "s\n";
    std::cout << "List: " << timeB << "s\n";
    /*COODDY_SUPPRESS*/
    return 0;
}
