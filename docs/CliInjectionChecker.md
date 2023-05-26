## Description

Untrusted data, such as user input, command-line argument, or file contents is passed to a function launching a process or executing commands in a shell (such as std::system).

Vulnerability

Because untrusted data can be manipulated, a potential attacker may modify the path to the executable, arguments with which the executable is called, or inject a payload into the shell command, by chaining a malicious command to an existing one. This can allow the attacker to alter the behavior of the program in unpredictable ways. In the worst case, such an attack leads to remote code execution.

Furthermore, this problem is compounded when the program is executed with elevated privileges. In this case, the attacker can do anything from stealing read-protected data like passwords and SSH keys to taking full control over a remote machine.

## Examples

```cpp
#include <cstdlib>

#include <iostream>

#include <string>
/// Compile a file passed in argv[1], using Microsoft Visual Studio compiler
int main(int argc, char** argv)
{
    std::string shell_environment = "\"C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat\"";
    std::string source_filename = argv[1];
    std::system((shell_environment + " && cl.exe " + source_filename).c_str());
}
```

One could exploit such code by sending `argv[1]` like `file.cpp && scp %userprofile%/Documents ...`, or other malicious payload.

## Related Guideline \ CWE

[CWE-78: Improper Neutralization of Special Elements used in an OS Command ('OS Command Injection')](https://cwe.mitre.org/data/definitions/78.md)

## List of command-line interface functions checked by default

* _execl
* _execle
* _execlp
* _execlpe
* _execv
* _execve
* _execvp
* _execvpe
* _popen
* _spawnl
* _spawnle
* _spawnlp
* _spawnlpe
* _spawnv
* _spawnve
* _spawnvp
* _spawnvpe
* _texecl
* _texecle
* _texeclp
* _texeclpe
* _texecv
* _texecve
* _texecvp
* _texecvpe
* _tpopen
* _tspawnl
* _tspawnle
* _tspawnlp
* _tspawnlpe
* _tspawnv
* _tspawnve
* _tspawnvp
* _tspawnvpe
* _tsystem
* _wexecl
* _wexecle
* _wexeclp
* _wexeclpe
* _wexecv
* _wexecve
* _wexecvp
* _wexecvpe
* _wpopen
* _wspawnl
* _wspawnle
* _wspawnlp
* _wspawnlpe
* _wspawnv
* _wspawnve
* _wspawnvp
* _wspawnvpe
* _wsystem
* CreateProcessA
* CreateProcessAsUserA
* CreateProcessAsUserW
* CreateProcessW
* CreateProcessWithLogonW
* CreateProcessWithTokenW
* execl
* execle
* execlp
* execlpe
* execv
* execve
* execvp
* execvpe
* g_spawn_async
* g_spawn_command_line_async
* g_spawn_command_line_sync
* g_spawn_sync
* popen
* posix_spawn
* posix_spawnp
* ShellExecuteA
* ShellExecuteExA
* ShellExecuteExW
* ShellExecuteW
* spawnl
* spawnle
* spawnlp
* spawnlpe
* spawnv
* spawnve
* spawnvp
* spawnvpe
* system
* WinExec
