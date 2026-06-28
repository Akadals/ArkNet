#include "InputHandler.h"
#include "ConsoleRenderer.h"

void SetInputPosition(SHORT x, SHORT y)
{
    //write(x, y, ">");
    g_pInputPosition = { (SHORT)(x + 2), y };
}

void GetInput()
{
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
            AkaNetCore::command.Excute(g_sInputBuffer);
            g_sInputBuffer.clear();
        }
        else if (key == '\b' && !g_sInputBuffer.empty())
            g_sInputBuffer.pop_back();
        else if (isprint(static_cast<unsigned char>(key)))
            g_sInputBuffer.push_back(key);
    }
}