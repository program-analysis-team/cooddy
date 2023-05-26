# Description

A function featuring a format string (like printf) is called with a non-matching set of format specifiers and arguments. This may lead to incorrect output or program crashes.

To be inspected by this checker, a function should have a "FormatString" annotation with a special data string specifying which kind of format function is being used. [More information on configuration below.](FmtSpecMismatchChecker.md)

# Problem kinds

When the amount of format arguments passed to the function is smaller than the amount of format specifiers, garbage data will be printed in place of format specifiers that did not receive an argument. In case of format specifiers that require a pointer, reading garbage data might lead to undefined behavior and crashes.


> ⚠️ This problem kind can be disabled in the profile by setting property "enabled" of kind "FMT.SPEC.TFA" to `false`.



<details>

<summary>Example
</summary>

```cpp
#include <stdio.h>
void foo(FILE* f, const char* firstName, const char* lastName) {
    // Not enough arguments for specifiers in format string.
    // Garbage data will be read from the stack, risking program crashes.
    fprintf(f, "%s %s", firstName, /* lastName */);
}
```

</details>

## Too many arguments for format string

When the amount of format arguments passed to the function is bigger than the amount of format specifiers, extra arguments will not be printed.


> ⚠️ This problem kind can be disabled in the profile by setting property "enabled" of kind "FMT.SPEC.TMA" to `false`.





<details>

<summary>Example
</summary>

```cpp
#include <cstdio>
string employeeName;
int employeeAge;
char employeeFavoriteLetter;
void foo(FILE* f) {
    // Due to a different amount of format specifiers and passed values, important information is not printed.
    std::fprintf(f, "%s, %d", employeeName.c_str(), employeeAge, employeeFavoriteLetter);
}
```

</details>

## Invalid format specifier

A format specifier consists of a percent "%" symbol, followed by several optional modifiers that change the way the argument is supposed to be printed, such as alignment, padding, type width, etc., and ending with a conversion specifier ("d", "u", "s", etc.). Some conversion specifiers are not compatible with some modifiers, for example you can add a length modifier `ll` to `%d` (producing `%lld` taking `long long int`) but you cannot do the same to `%s`. Such invalid format specifiers result in undefined behavior during program execution.

See more information on format specifiers: [std::printf, std::fprintf, std::sprintf, std::snprintf - cppreference.com](https://en.cppreference.com/w/cpp/io/c/fprintf)


> ⚠️ This problem kind can be disabled in the profile by setting property "enabled" of kind "FMT.SPEC.MISMATCH" to `false`.






<details>

<summary>Example
</summary>

```cpp
#include <stdio.h>
void foo(long int value) {
    // The correct length modifier for long int is actually lowercase 'l', meaning this code produces undefined behavior.
    printf("%Ld", value);
}
```

</details>

## Pointer-related type mismatch

Pointer-related type mismatches between a format specifier and an argument occur when one of the following is true:

* format specifier expects a pointer (such as "%s", "%p", "%n", etc.), but provided argument is not a pointer type;
* format specifier does not expect a pointer, but provided argument is a pointer type;
* format specifier expects a pointer, but provided argument is a wrong pointer type, (pointer to int instead of double, const instead of mutable, pointer to an integer of different byte width).
  These errors can result in a variety of consequences, such as garbage data being printed, unaligned reads, segmentation faults, etc.

> ⚠️ This problem kind can be disabled in the profile by setting property "enabled" of kind "FMT.SPEC.PTR" to `false`.






<details>

<summary>Example 1
</summary>

```cpp
#include <stdio.h>
void foo(const char* name, int age) {
    // By mistakenly passing age as the first argument instead of name, programmer caused
    // a memory access at value of age, probably causing a segmentation fault. 
    //                         ↓
    printf("Value at %s: %d", age, age);
}
```

</details>



<details>

<summary>Example 2
</summary>

```cpp
#include <stdio.h>
int serialize(char* name char* str, const int* count) {
    // count is const, but %n requires a pointer to mutable int to write to.
    sprintf(str, "%s%n", name, &count);
}
```

</details>



<details>

<summary>Example 3
</summary>

```cpp
#include <stdio.h>
int foo(wchar_t* data) {
    // %s expects a pointer to width of 1, but wchar_t has a width of 4 and needs %ls
    printf("%s", data);
}
```

</details>

## Signed/Unsigned type mismatch

When a format specifier expects a signed integer but the corresponding argument is unsigned (or vice versa), data from the argument will be reinterpreted (as if by `reinterpret_cast`) and it is possible that a wrong value will be printed (note that according to the C and C++ this mismatch invokes undefined behavior).


> ⚠️ This problem kind can be disabled in the profile by setting property "enabled" of kind "FMT.SPEC.SIGN" to `false`.






<details>

<summary>Example
</summary>

```cpp
#include <algorithm>

#include <array>

#include <cstdio>

#include <string>
using namespace std;
int getIndex(const string& text) {
    static array<string, 3> vegetables = {"cucumber", "potato", "squash"};
    auto it = find(vegetables.begin(), vegetables.end(), text);
    if (it == vegetables.end()) return -1;  // Not found
    return it - vegetables.begin();
}
int main() {
    // %u expects an unsigned integer, but if getIndex returns -1, an incorrect value will be printed.
    printf("%u", getIndex("tomato"));
}
```

</details>

## Other type mismatches

Remaining type mismatches include type mismatches between integers and floating point types, and type byte width mismatches.


> ⚠️ This problem kind can be disabled in the profile by setting property "enabled" of kind "FMT.SPEC.MISMATCH" to `false`.






<details>

<summary>Example 1
</summary>

```cpp
int foo(double x) {
    // %u expects an unsigned integer, but x has type double.
    printf("%u", x);
}
```

</details>



<details>

<summary>Example 2
</summary>

```cpp
int foo(uint64_t x) {
    // %u expects an unsigned integer of width 4 bytes, but x is 8 bytes wide.
    printf("%u", x);
}
```

</details>

# Configuration

This checker supports different kinds of format strings and corresponding arguments. When annotating your function, specify a `FormatString` annotation with a data field from one of the options below:

| Annotation             | Format string type                                      | Format descriptions                                                                                                                                |
|------------------------|---------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------|
| `FormatString::Printf` | printf-style (printf, fprintf, sprintf, printf_s, etc.) | Arguments are provided for reading, i.e. as values instead of pointers. [Reference](https://en.cppreference.com/w/cpp/io/c/fprintf)                |
| `FormatString::Scanf`  | scanf-style (scanf, fscanf, sscanf, etc.)               | Arguments are provided as writable pointers. [Reference](https://en.cppreference.com/w/cpp/io/c/fscanf)                                            |
| `FormatString::Scanfs` | scanf_s-style (scanf_s, fscanf_s, sscanf_s, etc.)       | Same as "Scanf", but format specifiers `%c" "%s" "%[` require an additional argument. [Reference (4-6)](https://en.cppreference.com/w/c/io/fscanf) |

By default, the checker will consider all variadic arguments as arguments to the format string. If you need to override this behavior, specify a `FormatStringArgs` annotation on the first format string parameter.

# List of functions checked by default

* `printf`
* `fprintf`
* `sprintf`
* `snprintf`
* `printf_s`
* `fprintf_s`
* `sprintf_s`
* `snprintf_s`
* `scanf`
* `fscanf`
* `sscanf`
* `scanf_s`
* `fscanf_s`
* `sscanf_s`
