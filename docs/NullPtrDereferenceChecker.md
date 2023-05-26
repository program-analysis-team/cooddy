## Description

A NULL pointer dereference occurs when the application dereferences a pointer that it expects to be valid, but is NULL, typically causing a crash or exit.

## Vulnerability

Null-pointer dereferences usually result in the failure of the process. These issues typically occur due to inappropriate pointer creating, usage in multiple threads, addressing fields of a structure pointer which points to null and so on.

A dereference before the null check can result in:

* a runtime error if the pointer was null
* unintended program results if a condition was written incorrectly
* unnecessary code generated if there is a redundant check

## Examples

**Confirmed examples.**

**Dereference of null pointer**

```cpp
int main()
{
    int* a = 0;
    int b = *a;
    return b;
}
```

**Dereference of structure null pointer**

```cpp
struct A {
    int b;
};
int main()
{
    struct A * a = 0;
    a->b = 1;
}
```

**Dereference null initialized struct member**

```cpp
typedef struct A {
    int* pointer;
} A;
int foo()
{
    A* a;
    memset(a, 0, sizeof(A));
    return *a->pointer;
}
int bar()
{
    A a;
    memset(&a, 0, sizeof(A));
    return *a.pointer;
}
```

## Limitations

Pointer arithmetic.

## Related guideline \ CWE

[CWE_476](https://cwe.mitre.org/data/definitions/476.md)

## How it may be adjusted

This Checker uses following annotations: [Deref](Annotations.md), [InitNull](Annotations.md). So you can use them to configure precision of this checker.

**Example of configuration**

**Example of changing existing function annotations**

By default, annotation on return value of function malloc, doesn't contain information on possible return of 0 value. It was done in purpose to avoid noisy reporting on return value of this function. Current value of function description:

```json
"malloc(malloc)": [["TakeResource"], []],
```

But if you need to check the return value of this function on possible 0 value you need to add annotation InitNull to function .cooddy/annotations.json like this:

```json
"malloc(malloc)": [["InitNull","TakeResource"], []],
```

**Example of adding new function annotation**

For example, we have following test-case:

```cpp
int undefined_local_memcpy(void * destination, const void * source, size_t num);
void foo() {
  int* ptr = 0;
  int source[5];
  undefined_local_memcpy(ptr,source,0x123);
}
```

By default, there will be no reporting of error on line 7, since there is no definition of function undefined_local_memcpy. So to make Cooddy detect this error you need to add annotation for function undefined_local_memcpy to describe that there is dereference performed on first and second argument. To add annotation of this user function you need to add new line to file .cooddy/annotations.json.

```json
"undefined_local_memcpy": [[],["Deref"],["Deref"],[]]
```
