#include "ConsoleManager.h"
void RunTUI()
{
	if (g_hHandle) return;
	g_hHandle = (HANDLE)_beginthreadex(NULL, 0, render_thread, NULL, 0, NULL);
}
