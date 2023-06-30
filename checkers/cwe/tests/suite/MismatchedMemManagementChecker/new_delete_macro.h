#include <new>

#define ALLOC_MEM new (std::nothrow)

#define FREE_MEM(ptr) if(ptr != nullptr) {delete ptr;}
