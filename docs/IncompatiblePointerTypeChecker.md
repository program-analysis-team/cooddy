## Description

There are several conditions for this checker:

* checker detects if there is a cast from smaller type to bigger (in bits) **OR** between different pointers one of which is float and another is integer;
* checker does not detect if:

    + there is a cast inside macro expansion;
    + there is a cast from `void*` pointer;
    + there is a cast to one byte size type pointers;
* checker detects only on pointers got by taking address on variable of trivial type. It means casting between declared pointer variables are skipped.

## Vulnerability

Modifying a variable through a pointer of an incompatible type can lead to unpredictable results as well as cause out-of-bounds memory access.

## Examples

**In the following C code there is a cast to incompatible type **by assigning pointer *&y* (unsigned short*) to the *x* variable (**unsigned int*)****

```cpp
int foo(int a, short b)
{
    unsigned short y = b;
    unsigned int* x = &y; //error here for &y pointer
    *x = a;
    return 0;
}
```

****In the following C code there is a cast to incompatible type **by assigning pointer *&f* (float*) to the *i* variable (**int*)******

```cpp
void foo()
{
    float f = 0.0f;
    int* i = (int*)&f; //error here for &f pointer
    (*i)++;
    printf("float is %f\n", f);
}
```

******In the following C code there is a cast to incompatible type **by passing pointer *&a* (unsigned short*) to the int* *p* argument of *bar* function********

```cpp
void bar(int* p)
{
    *p = 10;
}
void foo()
{
    unsigned short a;
    bar(&a); //error here for &a pointer
}
```

## Related guideline \ CWE

[CWE-843: Access of Resource Using Incompatible Type ('Type Confusion')](https://cwe.mitre.org/data/definitions/843.md)
