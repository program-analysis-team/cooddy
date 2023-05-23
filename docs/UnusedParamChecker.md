## Description
Checker detects parameters that are declared in function but don't used in expressions. 

## Examples
**Confirmed examples**




```cpp
int test(int a)
{
    return 0;
}
```


  



**Non-confirmed example**




```
struct B {
    int par;
};
class A {
public:
    A(B& b) : A(b.par) {};
    A(int val) : value(val){};
private:
    int value;
};
```
