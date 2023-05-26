## Description

On both stack and heap allocated areas. It may occur when one access structure field by name and an incorrect cast or incorrect memory allocation takes place beforehand.

## Vulnerability

The software performs operations on objects in memory, but taking in account C and C++ memory model, which is plain, one can read from or write to a memory location that is outside the intended boundary of the object. Languages like C and C++ allow direct addressing of memory locations and do not automatically ensure that these locations are valid for the object that is being referenced. This can cause read or write operations to be performed on memory locations that may be associated with other variables, data structures, or internal program data.

As a result, an attacker may be able to execute arbitrary code, alter the intended control flow, read sensitive information, or cause the system to crash.

## Examples

**Confirmed examples.**

```cpp
#include <malloc.h>

#include <stdlib.h>
struct Base {
    int field1;
};
struct Derived : public Base {
    int field2;
};
int foo(Base* b)
{
    int v = b->field1; // no error here
    return ((Derived*)b)->field2; // Type size Mismatch
}
int main()
{
   Base* b;
   b = (Base*)malloc(sizeof(Base));
   return foo(b);
}
```

```cpp
struct A {
    int x;
 int y;
};
struct B {
    int x;
 int y;
 int z;
};
void init(void* ptr){
    struct B *b = (struct B*) ptr;
    b->x = 0;
    b->y = 0;
    b->z = 0; // Sink
}
int main(int argc, char* argv[]){
    char cahrarr[2];
    init((void*) cahrarr);
    return 0;
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
