## Description
The product divides a value by zero.

## Vulnerability
Integer division by zero usually result in the failure of the process or an exception. It can also result in success of the operation, but gives an erroneous answer or lead to undefined behavior.

## Examples
**Confirmed examples.**
**Divide by integer variable which value is zero.**

```cpp
int main()
{
    int a = 2;
    int b = 0;
    int div = a / b;
    return 0;
}
```

## Related guideline \ CWE
[CWE_369](https://cwe.mitre.org/data/definitions/369.md)

## How it may be adjusted
This Checker uses following annotations: [Write](Annotations.md), [Divider](Annotations.md). So you can use them to configure precision of this checker.


**Example of configuration - Write annotation example**


For example, in the following test-case error will not be reported on line 5, since there is no definition of function undefined_local_write.




```cpp
int main()
{
    int a = 2;
    int b = 0;
    undefined_local_write(a / b);
    return 0;
}
```


 So to make Cooddy detect this error you need to add annotation for function undefined_local_write to describe that there is freeing of resource pointed by first argument is performed. To add annotation of this user function you need to add new line to file .cooddy/annotations.json.




```
"undefined_local_write": [["Write"]]
```
