## Description

Checks that there are no access to memory locations outside allocated structures. On both stack and heap allocated areas. It may occur when one access array item by index and index is below zero or above the array size, in case of incorrect cast and same cases.

## Vulnerability

The software performs operations on a memory buffer, but it can read from or write to a memory location that is outside the intended boundary of the buffer. Languages like C and C++ allow direct addressing of memory locations and do not automatically ensure that these locations are valid for the memory buffer that is being referenced. This can cause read or write operations to be performed on memory locations that may be associated with other variables, data structures, or internal program data.

As a result, an attacker may be able to execute arbitrary code, alter the intended control flow, read sensitive information, or cause the system to crash.

## Examples

**Confirmed examples.**

```cpp
#include <malloc.h>
int main()
{
    int* arr = (int*)malloc(10 * sizeof(int));
    int v = arr[9];  // no error here
    return arr[10];  // out of bounds
}
```

```cpp
int main()
{
    int arr[10] = {0};
    int v = arr[9];  // no error here
    return arr[11];  // out of bounds
}
```

```cpp
#include <stdint.h>
void foo(bool flag)
{
    uint8_t buff[22] = {0};
    uint16_t *p16 = ((uint16_t *)buff) + 1;
    uint32_t *p32 = ((uint32_t *)p16) + 1;
    uint64_t *p64 = ((uint64_t *)p32) + 1;
    uint64_t v = *p64;                   // no error here
    uint8_t v1 = *(uint8_t *)(p64 + 1);  // error here (2 + 4 + 8 + 8 + 1 > 22)
}
```

## Related guideline \ CWE

[CWE-119](https://cwe.mitre.org/data/definitions/119.md)

## How it may be adjusted

This Checker uses [Access](Annotations.md) annotation for dereference, array subscriptions and field access nodes, with specific data for each usage. It also uses [AllocSource](Annotations.md) to check actual size of allocated memory.

If you know that some **external** function (without source code in scope of analyzed project) performs memory allocation, you can add description as in example bellow. As for Access annotation, it is not supposed to be configured by user now.

**Example of adding new function annotation**

For example, we have following test-case:

```cpp
int undefined_local_malloc(void * allocated_memory_o, size_t size_bit );
void foo() {
    int* ptr = 0;
    int source[5];
    undefined_local_memcpy(ptr,source,0x123);
}
```

By default, there will be no reporting of error on line 7, since there is no definition of function undefined_local_memcpy. So to make Cooddy detect this error you need to add annotation for function undefined_local_memcpy to describe that there is dereference performed on first and second argument. To add annotation of this user function you need to add new line to file .cooddy/annotations.json.

```json
"undefined_local_malloc": [[],["AllocSource"],[]]
```
