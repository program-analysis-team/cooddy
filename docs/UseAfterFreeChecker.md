## Description.
If memory is used or referenced after it's been freed, or if it's freed twice, results can be unpredictable. Memory reference problems can cause the use of unexpected values, which in turn can cause programs to crash or execute arbitrary code.  

## Vulnerability
Memory reference issues can prove to be critical problems. Using previously freed memory can result in corruption of valid data or execution of arbitrary code, depending on the specific situation. When memory is freed, its contents can remain intact and accessible if it isn't reallocated or recycled. The data at the freed location may seem to be valid, but it can change unexpectedly, and cause unintended code behavior.


If memory is allocated to another pointer at some time after it's been freed and the original pointer is used again, it can point to a location in the new allocation. As the data is changed, it can corrupt the validly used memory and result in undefined actions. If a function pointer is overwritten with an address to valid code, a malicious user can cause execution of arbitrary code.


When a program frees the same memory twice, the memory management data structures become corrupted, causing the program to crash or return the same pointer in two later function calls. In this case, an attacker can achieve control over the data that's written into the doubly-allocated memory, which then becomes vulnerable to a buffer overflow attack.

## Examples
**Confirmed examples.**
**Call double free on the same memory address**

```cpp

#include "stdlib.h" 
void foo()
{
    int* a = (int*)malloc(sizeof(int));
    free(a);
    free(a);
}
```


**Call double delete on the same memory address**

```cpp
void foo()
{
    int* a = new int;
    delete(a);
    delete(a);
}
```


**Call delete free on the same memory address**

```cpp
void foo()
{
    int* a = new int;
    delete(a);
    free(a);
}
```

## Related and Contradictory Checkers
Related with double free checker

## Limitations
Unsupported case: recursion call

## Related guideline \ CWE
[CWE_415](https://cwe.mitre.org/data/definitions/415.md), [CWE_416](https://cwe.mitre.org/data/definitions/416.md)

## How it may be adjusted
This Checker uses following annotations: [Deref](Annotations.md), [FreeSource](Annotations.md). So you can use them to configure precision of this checker


**Example of configuration**
**FreeSource annotation example**


For example, in the following test-case error will not be reported on line 9, since there is no definition of function undefined_local_free.




```cpp

#include <stdlib.h>
void undefined_local_free(void* ptr);
int main ()
{
    int * a;
    a = (int*) calloc(100, sizeof(int));
    undefined_local_free(a);
    int b = *a;
    return b;
}
```


So to make Cooddy detect this error you need to add annotation for function undefined_local_free to describe that there is freeing of resource pointed by first argument is performed. To add annotation of this user function you need to add new line to file .cooddy/annotations.json. Read [here](Annotations.md) how to create it 




```
"undefined_local_free": [[], ["FreeSource"]]
```


**Deref annotation example**


For example, in the following test-case error will not be reported on line 9, since there is no definition of function undefined_func, as the result there is no information if dereference is performed of argument ptr.




```cpp

#include <stdlib.h>
void undefined_func(void* ptr);
int main ()
{
    int * a;
    a = (int*) calloc(100, sizeof(int));
    free(a);
    int b = *a;
    return b;
}
```


So to make Cooddy detect this error you need to add annotation for function undefined_func to describe that there dereference is performed of argument ptr. To add annotation of this user function you need to add new line to file .cooddy/annotations.json.




```
"undefined_func": [[], ["Deref"]]
```
