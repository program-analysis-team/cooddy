## Description

The application attempts to return a memory resource to the system, but it calls a release function that is not compatible with the function that was originally used to allocate that resource.

## Vulnerability

This weakness can be generally described as mismatching memory management routines, such as:

* The memory was allocated on the stack (automatically), but it was deallocated using the memory management routine free() ( [CWE-590](https://cwe.mitre.org/data/definitions/590.md)), which is intended for explicitly allocated heap memory.

* The memory was allocated explicitly using one set of memory management functions, and deallocated using a different set. For example, memory might be allocated with malloc() in C++ instead of the new operator, and then deallocated with the delete operator.

When the memory management functions are mismatched, the consequences may be as severe as code execution, memory corruption, or program crash. Consequences and ease of exploit will vary depending on the implementation of the routines and the object being managed.

Examples

**Confirmed examples.** 

**attempt to delete memory, allocated by malloc**

```cpp
#include <stdlib.h>
void foo(){
    int *ptr;
    ptr = (int*)malloc(sizeof(int));
    delete ptr;                      // <- report error here
}
```

**attempt to free memory, allocated on stack**

```cpp
#include <stdlib.h>
void foo()
{
    int ptr[5];
    free(ptr);                   // <- report error here
}
```

**attempt to delete memory ,allocated by array**

```cpp
class BarObj {
public:
    BarObj() = default;
};
void foo()
{
    BarObj *ptr = new BarObj[5];
    int c =123;
    delete ptr;                   // <- report error here
}
```

| User Data | Function of allocation  | Function of deallocation |
|-----------|-------------------------|--------------------------|
| 1         | malloc, calloc, realloc | free                     |
| 2         | operator new            | operator delete          |
| 3         | operator new[]          | operator delete[]        |

## Related guideline \ CWE

[CWE-763](https://cwe.mitre.org/data/definitions/763.md), [CWE-762](https://cwe.mitre.org/data/definitions/762.md), [CWE-590](https://cwe.mitre.org/data/definitions/590.md)

## How it may be adjusted

This Checker uses [AllocSource](Annotations.md) and [FreeSink](Annotations.md) to check actual size of allocated memory.

If you know that some **external** function (without source code in scope of analyzed project) performs memory allocation or free memory, you can add description as in example bellow.

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

By default, there will be no reporting of error on line 7, since there is no definition of function undefined_local_memcpy. So to make Cooddy detect this error you need to add annotation for function undefined_local_memcpy to describe that there is dereference performed on first and second argument. To add annotation of this user function you need to add new line to file .cooddy/huawei-annotations.json.

```json
"undefined_local_malloc": [[],["AllocSource::1"],[]]
```

**Example of adding custom lock/unlock resource functions**

For example, we have a pair of custom functions which lock/unlock some resource.

```cpp
void my_mutext_lock1(my_mutex* m);
void my_mutext_unlock1(my_mutex* m);
void my_mutext_lock2(my_mutex* m);
void my_mutext_unlock2(my_mutex* m);
my_mutext_lock1(m);
my_mutext_unlock2(m);  // error: wrong unlock function is used
```

To detect this kind of defects we should give different group of functions different annotation numbers in annotations.json

```json
"my_mutext_lock1": [[],["LockResource::10"],[]],
"my_mutext_unlock1": [[],["UnlockResource::10"],[]],
"my_mutext_lock2": [[],["LockResource::20"],[]],
"my_mutext_unlock2": [[],["UnlockResource::20"],[]]
```
