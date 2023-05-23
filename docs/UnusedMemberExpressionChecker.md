## Description
Checker detects class/struct members that are not used in expressions. 

## Examples
**Confirmed examples**




```cpp
struct A {
    int x = 0;  // no error here
    int y = 0;  // error here
};
union B {
    int x;
    int y;
};
int main()
{
    A a;
    B b;
    return a.x + a.y;
}
```


  



**Non-confirmed example**




```cpp
typedef struct READ4resok {
    bool eof = false;
    struct {
        int offset = 0;
        int nread = 0;
    } data;
} READ4resok;
int main()
{
    READ4resok a;
    bool f = a.eof; // unused member
    return a.data.offset;
}
```

## Related and Contradictory Checkers
[UnusedVariablesChecker](UnusedVariableChecker.md)

## Related guideline \ CWE
-

## How it may be adjusted
This checker uses following annotations: *Write*.
