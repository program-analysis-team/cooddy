## Description
Contains the most critical checkers


CWE Checkers are used to found vulnerabilities from Common Weakness Enumeration list. This is always about some critical event, that may subject program to crash, corrupt data, became an entry point for hacking.

## How to enable this group
By default the checkers from this category are enabled. 

## How to disable this group
As was mentioned before, by default the checkers from this category are enabled. To disable CWE Checkers you can do following:



* you can ignore all cwe-checkers by removing cwe.profile from <cooddy_installation_path>/.cooddy/default.profile.

## Checkers
* [BufferMaxSizeChecker](BufferMaxSizeChecker.md)
* [CliInjectionChecker](CliInjectionChecker.md)
* [DivisionByZeroChecker](DivisionByZeroChecker.md)
* [DoNotReturnAddrOfLocalVarChecker](DoNotReturnAddrOfLocalVarChecker.md)
* [FmtSpecMismatchChecker](FmtSpecMismatchChecker.md)
* [FormatStringUsageChecker](FormatStringUsageChecker.md)
* [IncompatiblePointerTypeChecker](IncompatiblePointerTypeChecker.md)
* [IntegerOverflowChecker](IntegerOverflowChecker.md)
* [LoopLimitTaintChecker](LoopLimitTaintChecker.md)
* [MemoryAllocationSizeChecker](MemoryAllocationSizeChecker.md)
* [MemoryAndResourceLeakChecker](MemoryAndResourceLeakChecker.md)
* [MismatchedMemManagementChecker](MismatchedMemManagementChecker.md)
* [NullPtrDereferenceChecker](NullPtrDereferenceChecker.md)
* [OutOfBoundsChecker](OutOfBoundsChecker.md)
* [TypeSizeMismatchChecker](TypeSizeMismatchChecker.md)
* [UninitializedMemoryUsageChecker](UninitializedMemoryUsageChecker.md)
* [UseAfterFreeChecker](UseAfterFreeChecker.md)


  

## Link to the corresponding standards
CWE:


[CWE-476](https://cwe.mitre.org/data/definitions/476.md) , [CWE-824](https://cwe.mitre.org/data/definitions/824.md) , [CWE-457](https://cwe.mitre.org/data/definitions/457.md), [CWE-476](https://cwe.mitre.org/data/definitions/476.md) , [CWE-824](https://cwe.mitre.org/data/definitions/824.md) , [CWE-457](https://cwe.mitre.org/data/definitions/457.md), [CWE-134](https://cwe.mitre.org/data/definitions/134.md) , [CWE-401](https://cwe.mitre.org/data/definitions/401.md), [CWE-20](https://cwe.mitre.org/data/definitions/20.md), [CWE-190](https://cwe.mitre.org/data/definitions/190.md), [CWE-416](https://cwe.mitre.org/data/definitions/416.md), [CWE-762](https://cwe.mitre.org/data/definitions/762.md), [CWE-119](https://cwe.mitre.org/data/definitions/119.md), [CWE-562](https://cwe.mitre.org/data/definitions/562.md), [CWE-806](https://cwe.mitre.org/data/definitions/806.md)
