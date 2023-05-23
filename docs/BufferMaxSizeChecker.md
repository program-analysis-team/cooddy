## Description
The program copies an input buffer to an output buffer without verifying that the size of the input buffer is less than the size of the output buffer, leading to a buffer overflow.


The program assess buffer with offset without verifying the size of the buffer.

## Vulnerability
A buffer overflow condition exists when a program attempts to put more data in a buffer than it can hold, or when a program attempts to put data in a memory area outside of the boundaries of a buffer. The simplest type of error, and the most common cause of buffer overflows, is the "classic" case in which the program copies the buffer without restricting how much is copied. Other variants exist, but the existence of a classic overflow strongly suggests that the programmer is not considering even the most basic of security protections.

## Examples
**Confirmed examples.**




```cpp
int memmove_s(char* dstBuff, int dstMax, char* srcBuff, int srcSize);
int main()
{
    char dstBuff[2];
    char srcBuff[] = "Some data";
    memmove_s(dstBuff, sizeof(dstBuff), srcBuff, sizeof(srcBuff));
    return 0;
}
```

## Related guideline \ CWE
[CWE-120](https://cwe.mitre.org/data/definitions/120.md)

## How it may be adjusted
This Checker uses following annotations: MemorySize, [BuffSize](Annotations.md), [Value](Annotations.md). If you use some specific toolchain, you should annotate such functions as mentioned in following example


**MemorySize annotation example**


In the following test-case function copies string.




```
struct A {
    char buff[20];
    char buff2[10];
};
void a(uint32_t size) {
    A buffer;
    if(size < 20)
        strncmp(buffer.buff, buffer.buff2, size);  // error here
}
```


So buff2 has not enough size to compare 10-19 characters


  



**BuffSize annotation example**


For example, in the following test-case function undefined_local_memcpy will not be analyzed as function that copy buffers




```cpp
int undefined_local_memcpy_s(void * destination, size_t dest_size const void * source, size_t source_size);
void foo() {
    int* ptr = (int*) malloc(3 * sizeof(int));
    int source[5];
    undefined_local_memcpy(ptr, 3, source, 5); 				// <- error here 
}
```


 So to make Cooddy analyze such functions you need to add annotation for function undefined_local_memcpy to describe that there is coping of buffers with defined sizes. To add annotation of this user function you need to add new line to file .cooddy/annotations.json.




```
"undefined_local_memcpy": [[], ["Deref","Write:*","InitMemory"], [], ["Deref"], ["BuffSize::2"]],
```


 Here after double column you should provide an index of parameter that represents destination size. Index starts from 1.
