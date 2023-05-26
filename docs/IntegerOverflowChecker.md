## Description

The software performs a calculation that can produce an integer overflow or wraparound, when the logic assumes that the resulting value will always be larger or smaller than the original value. This can introduce other weaknesses when the calculation is used for resource management or execution control.

## Vulnerability

An integer overflow or wraparound occurs when an integer value is incremented or decremented to a value that is too large or too smal to store in the associated representation. When this occurs, the value may wrap to become a very small or negative number. While this may be intended behavior in circumstances that rely on wrapping, it can have security consequences if the wrap is unexpected. This is especially the case if the integer overflow can be triggered using user-supplied inputs. This becomes security-critical when the result is used to control looping, make a security decision, or determine the offset or size in behaviors such as memory allocation, copying, concatenation, etc.

## Supported cases:

The current implementation of the checker can report integer overflow problem in two cases:

1. The result of any arithmetic operation, which can lead to integer overflow, is used as a parameter of annotated with CheckOverflow function.

> ⚠️ By default only 'printf' function is annotated with CheckOverflow annotation



> ⚠️ The problem is reported only if arithmetic operation operands are propagated from constants or untrusted data sources.


Example:

```cpp
#include <stdio.h>

#include <stdlib.h>
void bad()
{
    char data;
    data = ' ';
    fscanf(stdin, "%c", &data); /* fscanf marks data as untrusted */
    if (data > 0) /* ensure we won't have an underflow */
    {
        /* POTENTIAL FLAW: if (data*2) > CHAR_MAX, this will overflow */
        char result = data * 2;
        printf("%c", result); /* result is used in annotated with CheckOverflow function */
    }
}
```

2. An iterator of the loop is decremented inside the loop and this operation can lead to integer overflow

Example:

```cpp
void bad()
{
    int arr[10];
    for (unsigned i = 9; i >= 0; --i) {  // underflow when i=0
        arr[i] = i;
    }
}
void good()
{
    int arr[10];
    for (unsigned i = 9; i > 0; --i) {  // no problem here
        arr[i] = i;
    }
}
```

## Related guideline \ CWE

[CWE-190](https://cwe.mitre.org/data/definitions/190.md)

[CWE-191](https://cwe.mitre.org/data/definitions/191.md)

## How it may be adjusted

This Checker uses [CheckOverflow](Annotations.md) annotation as a marker of places in the source code where to check potential integer overflows.

**Example of adding new function annotation**

For example, we have the following test-case:

```cpp
1  void* my_malloc(size_t size);
2   
3  void foo() {
4      size_t size = 0;
5      fscanf(stdin, "%d", &size);
6      size += sizeof(int);
7      my_malloc(size);
8  }
```

By default, there will be no reporting of problem on the line 6, since there is no CheckOverflow annotation of the function `my_malloc`.

So to make Cooddy detect this problem you should add annotation for function 'my_malloc' to the file .annotations.json (see [Annotations](Annotations.md) for details).

```json
"my_malloc": [[],["CheckOverflow"]]
```
