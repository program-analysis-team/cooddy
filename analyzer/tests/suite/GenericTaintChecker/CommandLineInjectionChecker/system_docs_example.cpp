#include <cstdlib>
#include <string>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

/// Compile a file passed in argv[1], using Microsoft Visual Studio compiler
int EXTER_ATTACK main(int argc, char** argv)
{
    std::string shell_environment =
        "\"C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat\"";
    std::string source_filename = argv[1];

    std::system((shell_environment + " && cl.exe " + source_filename).c_str());
}
