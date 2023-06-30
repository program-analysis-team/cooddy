#include <stdio.h>
#include <stdlib.h>

int m_position = 10;
int m_end = 15;

int check_func(int check_val)
{
    ++m_position;
    int end;
    do {
        while ((m_position != m_end) && (m_position != check_val)) {
            ++m_position;
        }
        if (m_position == m_end) {
            end = m_position;
            break;
        }
        if (m_position == check_val) {
            end = m_position - 2;
            break;
        }
    } while (true);

    return m_end - end;
}

int main()
{
    int value = 13;
    return 0;
}
