#include <iostream>
#include <vector>

int foo(const std::vector<int>& vec) {
    for (auto item : vec) {
        std::cout << "It is range: " << item << std::endl;
    }

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << "It is for: " << vec[i] << std::endl;
    }

    {
        size_t i = 0;
        while (i < vec.size()) {
            std::cout << "It is while: " << vec[i] << std::endl;
            ++i;
        }
    }

    {
        size_t i = 0;
        do {
            std::cout << "It is do while: " << vec[i] << std::endl;
            ++i;
        } while (i < vec.size());
    }
}
