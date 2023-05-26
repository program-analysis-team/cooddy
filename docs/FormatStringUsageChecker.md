## Description

The software uses a function that accepts a format string as an argument, but the format string originates from an untrusted data, such as user input, command-line argument, or file contents.

## Vulnerability

When an attacker can modify an externally-controlled format string, this can lead to buffer overflows, denial of service, or data representation problems.

## Examples

```cpp
int main(int argc, char **argv){
    char buf[128];
    ...
    snprintf(buf, 128, argv[1]);
    ...
}
```

This code allows an attacker to view the contents of the stack and write to the stack using a command line argument containing a sequence of formatting directives.

## Related Guideline \ CWE

[CWE-134: Use of Externally-Controlled Format String](https://cwe.mitre.org/data/definitions/134.md)

## List of functions checked by default

* printf
* printf_s
* scanf
* snprintf
* snprintf_s
* sprintf
* sprintf_s
* sscanf
* sscanf_s
* swprintf
* swprintf_s
* swscanf
* swscanf_s
* vfprintf
* vfprintf_s
* vfscanf
* vfscanf_s
* vfwprintf
* vfwprintf_s
* vfwscanf
* vfwscanf_s
* vscanf
* vsnprintf
* vsnprintf_s
* vsnwprintf
* vsprintf
* vsprintf_s
* vsscanf
* vswprintf
* vswprintf_s
* vswscanf
* vwprintf
* vwprintf_s
* vwscanf
* vwscanf
* wprintf
* wprintf_s
* wscanf
