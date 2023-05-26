## Description

The software uses or accesses a resource that has not been initialized.

## Vulnerability

When reusing a resource such as memory or a program variable, the original contents of that resource may not be cleared before it is sent to an untrusted party.

The uninitialized resource may contain values that cause program flow to change in ways that the programmer did not intend.

## Examples

**Confirmed examples.**

**Usage of uninitialized pointer**

```cpp
void uninit_pointer_001()
{
    int a = 5;
    int *p;
    int ret;
    ret = *p;
}
```

**Usage of uninitialized variable**

```cpp
int main()
{
    int a, b;
    int c = 1 + a;
}
```

## Related guideline \ CWE

[CWE_908](https://cwe.mitre.org/data/definitions/908.md)

## How it may be adjusted

This Checker uses following annotations: [Read](Annotations.md). Annotation for function will be added by default, if there is no body of function and pointer is non const.
