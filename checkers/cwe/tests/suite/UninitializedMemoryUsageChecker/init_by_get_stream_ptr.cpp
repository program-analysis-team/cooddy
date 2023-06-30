#include <sstream>
#include <string>

int test(std::istream & s)
{
    std::string _command_line;

    char ch[5];
    while (s.get(ch,5))
    {
        if (ch[0] == '\n')
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
