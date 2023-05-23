## Description
This checker detects tree kind of issues:



* Missing release of memory after effective lifetime.
* Missing close of descriptor or handle after effective lifetime.
* Missing unlock of resource.

## Vulnerability
The software does not release a resource after its effective lifetime has ended, i.e., after the resource is no longer needed.  
When a resource is not released after use, it can allow attackers to cause a denial of service by causing the allocation of resources without triggering their release. Frequently-affected resources include memory, file descriptors, mutexes, etc.

## Examples
**Confirmed examples.**
**The following C function leaks a block of allocated memory if the call to read() does not return the expected number of bytes**

```cpp
char* getBlock(int fd)
{
    char* buf = (char*)malloc(BLOCK_SIZE);
    if (!buf) {
        return nullptr;
    }
    if (read(fd, buf, BLOCK_SIZE) != BLOCK_SIZE) {
        return nullptr;
    }
    return buf;
}
```


**The following C function does not close the file handle it opens if an error occurs. If the process is long-lived, the process can run out of file handles**

```cpp
int decodeFile(char* fName)
{
    char buf[BUF_SZ];
    FILE* f = fopen(fName, "r");
    if (!f) {
        printf("cannot open %s\n", fName);
        return DECODE_FAIL;
    } else {
        while (fgets(buf, BUF_SZ, f)) {
            if (!checkChecksum(buf)) {
                return DECODE_FAIL;
            } else {
                decodeBlock(buf);
            }
        }
    }
    fclose(f);
    return DECODE_SUCCESS;
}
```


**The following C function does not close the mutex in all branches. It may lead to deadlocks.**

```cpp
struct AA {
    pthread_mutex_t lock;
};
void foo(struct AA *a)
{
    pthread_mutex_lock(&a->lock);
    if (some_func()) {
        return;
    }
    pthread_mutex_unlock(&a->lock);
}
```


  

## Related guideline \ CWE
[CWE-772: Missing Release of Resource after Effective Lifetime](https://cwe.mitre.org/data/definitions/772.md)


[CWE-401: Missing Release of Memory after Effective Lifetime](https://cwe.mitre.org/data/definitions/401.md)


[CWE-775: Missing Release of File Descriptor or Handle after Effective Lifetime](https://cwe.mitre.org/data/definitions/775.md)


  

## How it may be adjusted
This Checker uses annotations: AllocSource, FreeSink. If you use some specific toolchain, you should annotate such functions with these annotations.


**Memory leak example**


 For example, there are declarations of custom functions for allocating/deallocating memory.




```
void *custom_alloc_func(size_t size);
void custom_dealloc_func(void* ptr);
```


To detect cases with these function user has to annotate them in .annotations.json




```
{
  "custom_alloc_func(custom_alloc_func)": [["AllocSource::1"], []],
  "custom_dealloc_func(custom_dealloc_func)": [[], ["FreeSink::1"]] 
}
```


If function allocates memory, but there is no need to check for deallocate (custom memory management) use ["AllocSource::0"] annotation 


**Missing close of file or release descriptor**


For example, there are declarations of custom functions for opening/closing file.




```
void custom_open(uint32_t fd);
void custom_close(uint32_t fd);
```


To detect cases with these function user has to annotate them in .annotations.json




```
{
  "custom_open(custom_open)": [[], ["AllocDescriptor::4"]],
  "custom_close(custom_close)": [[], ["FreeDescriptor::4"]] 
}
```


FileOpen annotation is added to report correct defect description. The number in AllocSource and FreeSource annotations is used to check correct pairing of open/close functions. All predefined file functions marked as 4. So if file opened by custom open function can not be closed by standard close file functions use different pair of the same numbers in annotations declaration.


**Missing close of resource (mutex) example**


For example, there are declarations of custom functions for locking and unlocking mutex.




```
void custom_lock(pthread_mutex_t *);
void custom_unlock(pthread_mutex_t *);
```


To detect cases with these function user has to annotate them in .annotations.json




```
{
  "custom_lock(custom_lock)": [[], ["LockResource::5"]],
  "custom_unlock(custom_unlock)": [[], ["UnlockResource::5"]] 
}
```


The number in LockResource and UnlockResource annotations is used to check correct pairing of functions. The functions marked with annotaions with the same number can be unlocked with any unlock function from the group.
