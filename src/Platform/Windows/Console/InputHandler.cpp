#include "InputHandler.h"
#include "ConsoleRenderer.h"

namespace
{
    COORD g_pInputPosition;
}
void set_input_position(COORD t_position)
{
    g_pInputPosition = t_position;
}

void get_input()
{
    while (g_sInputBuffer.size() > g_consoleSize.X - 6)
        g_sInputBuffer.pop_back();
    while (_kbhit())
    {
        char key = _getch();

        if (key == 0 || key == -32)
        {
            key = _getch();
            if (key == 72)
            {
            }
            else if (key == 80)
            {
            }
            continue;
        }

        if (key == '\r')
        {
            //AkaNetCore::command.Excute(g_sInputBuffer);
            g_sInputBuffer.clear();
        }
        else if (key == '\b' && !g_sInputBuffer.empty())
        {
            g_sInputBuffer.pop_back();
        }
        else if (isprint(static_cast<unsigned char>(key)))
        {
            if (g_sInputBuffer.size() >= g_consoleSize.X - 6) continue;
            g_sInputBuffer.push_back(key);
        }
    }
    //write({ 4,static_cast<SHORT>(g_consoleSize.Y - 2) }, g_sInputBuffer);
}