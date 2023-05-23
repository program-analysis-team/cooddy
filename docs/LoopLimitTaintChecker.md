## Description
The product does not properly check inputs that are used for loop conditions, potentially leading to a denial of service or other consequences because of excessive looping.

## Examples
```

#include <iostream>
void EXTER_ATTACK foo(int count) {
    for (int i = 0; i < count; i++) {  // error here - count is untrusted and isn't checked before using in loop condition
    }
}
void bar() {
    int x = 0;
    std::cin >> x;
    int i = 0;
    do {
        i++;
    } while (i < x); // error here - x is untrusted and isn't checked. Perform input validation.
}
```

## Related guideline \ CWE
[CWE-606: Unchecked Input for Loop Condition](https://cwe.mitre.org/data/definitions/606.md)

## Potential Mitigations
Do not use user-controlled data for loop conditions.


Perform input validation.
