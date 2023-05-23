## Description
The program allocates memory in heap without verifying that the size of allocating memory is less than INT32_MAX.

## Vulnerability
The product allocates memory based on an untrusted, large size value, but it does not ensure that the size is within expected limits, allowing arbitrary amounts of memory to be allocated. 


Not controlling memory allocation can result in a request for too much system memory, possibly leading to a crash of the application due to out-of-memory conditions, or the consumption of a large amount of memory on the system.

## Examples
**Confirmed examples.**




```

#include <stdint.h>

#include <stdlib.h>
int EXTER_ATTACK foo1(size_t size)
{
    void* ptr = nullptr;
    ptr = malloc(size);  		// error here - size is untrusted and isn't checked before allocation 
    free(ptr);
}
size_t bar();
void foo2()
{
    auto size = bar();
    void* ptr = malloc(size);  // error here - size is untrusted and isn't checked before allocation 
    free(ptr);
}
int EXTER_ATTACK foo3(size_t size)
{
    void* ptr = nullptr;
    if (size < INT32_MAX) {
        ptr = malloc(size);  	// no error here
    }
    free(ptr);
}
```

## Related guideline \ CWE
[CWE-789: Memory Allocation with Excessive Size Value](https://cwe.mitre.org/data/definitions/789.md)
