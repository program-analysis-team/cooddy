#include <sstream>
#include <string>

int test(std::istream & s)
{
    std::string _command_line;

    char ch;
    //read in the command line
    while (s.get(ch))
    {
        if (ch == '\n')
        {
            break;
        }
        else
        {
            _command_line += ch;
        }
    }

    return 0;
}
