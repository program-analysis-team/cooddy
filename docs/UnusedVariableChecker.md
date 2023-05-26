## Description

Checker detects variables that are declared in function scope but don't used in expressions.

## Examples

**Confirmed examples**

```cpp
int foo(){
    int aValue = 0;
    int bValue = 0;
    return aValue + bValue;
}
```

**Non-confirmed example**

```cpp
int foo(){
    int aValue = 0;
    int bValue = 0; // Unused local variable
    return aValue;
}
```

## Related and Contradictory Checkers

[UnusedMemberExpressionChecker](UnusedMemberExpressionChecker.md)

## Related guideline \ CWE
-

## How it may be adjusted

This checker uses following annotations: *Write*.
