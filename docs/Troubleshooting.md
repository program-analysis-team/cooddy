All these errors can be found in Cooddy analysis log.

## Failed to parse source files

If failed to parse counter more than 0, then inspect file and examine exact parse error

## Fatal Errors

Fatal errors may occur in a number of different [cases](https://gist.github.com/micbou/2ff098c0eb8001fdd070630a4bd1b3c4) of clang compiler or in case of external error.

The most common one is unresolved **#include directives.** Mostly it can happen due to wrong analysis environment, which has to be similar to build environment, or malformed compile_commands.json file which has wrong or has no information on header files location.

Another problem is exceeding the number of allowed syntax error.

You can examine log to get this information. If header file is not found

## Undefined functions number

The number of functions without definitions. This may occur if some of used functions are not defined (has no body definitions). Mostly it are library functions which linked as static or dynamic libraries and has only header files with function declarations. For such functions Cooddy analysis engine cannot make any assumptions and behaviour of these functions can be emulated with [annotations](Annotations.md).

Also, it can happen if your compile_commands.json has no compile commands for part of source code files.

## Z3 timeout

Count of times when z3 solver is not finished due to timeout. You can try restart analysis, because there are some random seed in solver or z3 solving time is very close to hardcoded limit. In any time it occur it may be the reason why some Warnings are not found.

## CFG build failed number

Count of case when it is failed to create Control Flow Graph. Like usage of goto instruction, switch statement, lambdas and so on. This is a very, very rare situation. Better try to find in a log message "Can't build CFG for function: <your function name>" and report to the team to let us improve an engine.

## Unknown compiler options

The counter of unknown options and options themselves. this may occur because of usage compiler options that are not supported by clang or gcc, or options introduced in latest versions that are not supported by Cooddy yet.
