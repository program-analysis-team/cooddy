## Description
A function returns the address of a stack variable, which will cause unintended program behavior, typically in the form of a crash.

## Vulnerability
Because local variables are allocated on the stack, when a program returns a pointer to a local variable, it is returning a stack address. A subsequent function call is likely to re-use this same stack address, thereby overwriting the value of the pointer, which no longer corresponds to the same variable since a function's stack frame is invalidated when it returns. At best this will cause the value of the pointer to change unexpectedly. In many cases it causes the program to crash the next time the pointer is dereferenced.

## Examples
**Confirmed examples.**




```cpp
void foo(int *&x)
{
    int z = 42;
    x = &z;
}
int main()
{
    int *x = nullptr;
    foo(x);
    return *x;
}
```

## Related guideline \ CWE
[CWE-562](https://cwe.mitre.org/data/definitions/562.md)

## How it may be adjusted
This Checker uses following annotations: [Write](Annotations.md), [Alias](Annotations.md), [AddrOfLocalVar](Annotations.md). 


**Write annotation example**


For example, in the following test-case error will not be reported on line 5, since there is no definition of function undefined_assign_b_to_a.




```cpp
void undefined_assign_b_to_a(int& a, int& b); // contains in example a = b;
void foo(int* a) {
  int val;
  int* b = &val;
  undefined_assign_b_to_a(a, b);            // <- a will contain address of val
}
```


  



 So to make Cooddy detect this error you need to add annotation for function undefined_assign_b_to_a to describe that there is alias and write on 1-st argument. To add annotation of this user function you need to add new line to file .cooddy/annotations.json.




```
"undefined_assign_b_to_a": [[], ["Write:&", "Alias:2"], []];
```
