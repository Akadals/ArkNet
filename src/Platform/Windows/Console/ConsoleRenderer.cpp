#include "ConsoleRenderer.h"

constexpr size_t MAX_INFO_BOX_SIZE = 55;
constexpr size_t MIN_INFO_BOX_SIZE = 27;

namespace
{
	std::vector<std::wstring> g_frameBuffer;
	static constexpr wchar_t panelFrames[] =
	{
		L'─', L'│', L'╭', L'╮',
		L'╯', L'╰', L'├', L'┬',
		L'┤', L'┴'
	};
	static constexpr wchar_t usageBars[] =
	{
		L'▁',L'▂',L'▃',L'▄',L'▅',L'▆',L'▇',L'█'
	};
	size_t infoBoxSize = 30;
}

void set_info_box_size(size_t t_size)
{
	infoBoxSize = t_size;
	infoBoxSize = std::clamp(
		infoBoxSize, 
		MIN_INFO_BOX_SIZE,
		MAX_INFO_BOX_SIZE);
}

void render_panel(COORD t_start, COORD t_size, std::wstring_view t_title, WORD t_attribute)
{
	SHORT i = 0;

	write(
		t_start,
		{ &panelFrames[2],1 },
		t_attribute);
	for (i = 1; i < t_size.X + 1;)
		write(
			t_start.X + i++, t_start.Y,
			{ &panelFrames[0],1 },
			t_attribute);
	write(
		t_start.X + t_size.X + 1, t_start.Y,
		{ &panelFrames[3],1 },
		t_attribute);
	for (i = 1; i < t_size.Y + 1;)
	{
		write(
			t_start.X, t_start.Y + i,
			{ &panelFrames[1],1 },
			t_attribute);
		write(
			t_start.X + t_size.X + 1, t_start.Y + i++,
			{ &panelFrames[1],1 },
			t_attribute);
	}
	write(
		t_start.X, t_start.Y + t_size.Y + 1,
		{ &panelFrames[5],1 },
		t_attribute);
	for (i = 1; i < t_size.X + 1;)
		write(
			t_start.X + i++, t_start.Y + t_size.Y + 1,
			{ &panelFrames[0],1 },
			t_attribute);
	write(
		t_start.X + t_size.X + 1, t_start.Y + t_size.Y + 1,
		{ &panelFrames[4],1 },
		t_attribute);

	if (t_title.empty()) return;
	write(
		t_start.X + 2, 
		t_start.Y,
		L" ",
		t_attribute);
	write(
		t_start.X + 3
		, t_start.Y,
		t_title);
	write(
		t_start.X + 3 + t_title.size(),
		t_start.Y,
		L" ",
		t_attribute);
}

void render_panel(COORD t_start, SHORT x, SHORT y, std::wstring_view t_title, WORD t_attribute)
{
	render_panel(t_start, { x,y }, t_title, t_attribute);
}

void render_panel()
{
	render_panel({ 0,0 },
		g_consoleSize.X - 2, 2);
	render_panel({ 0,4 },
		{ 137,2 },
		L"SERVER INFO");
	render_panel({ 0,8 },
		{ 35,5 },
		L"MEMORY");
	render_panel({ 0,15 },
		{ 35,5 },
		L"CPU");
	render_panel({ 0,22 },
		{ 65,9 },
		L"DATABASE");
	render_panel({ 0,33 },
		{ 35,10 },
		L"PROCESS");
	render_panel({ 37,8 },
		{ 28,12 },
		L"JOB QUEUE");
	render_panel({ 37,33 },
		{ 28,10 },
		L"EVENT");
	render_panel({ 0,45 },
		{ 65,1 },
		L"ALERT");
	render_panel({ 67,8 },
		70, 12,
		L"CLIENT LIST");
	render_panel({ 67,22 },
		{ 70,4 },
		L"CONNECTION");
	render_panel({ 67,28 },
		70, 12,
		L"THREAD");
	render_panel({ 67,42 },
		70, 4,
		L"PERFORMANCE");
	render_panel({ 139,4 },
		g_consoleSize.X - 141, 3,
		L"NETWORK");
	render_panel({ 139,9 },
		g_consoleSize.X - 141, 6,
		L"PACKET CAPTURE");
	render_panel({ 139,17 },
		g_consoleSize.X - 141, g_consoleSize.Y - 31,
		L"LOG OUTPUT");
	render_panel({ 139,static_cast<SHORT>(g_consoleSize.Y - 12) },
		g_consoleSize.X - 141, 10,
		L"COMMAND LOG");
	render_panel({ 0,static_cast<SHORT>(g_consoleSize.Y - 3) },
		137, 1,
		L"COMMAND");
}

void render(int t_panel)
{
	if (t_panel & PANEL_NETWORK)
	{
		write({ 1,1 }, L"PANEL_NETWORK");
	}
	if (t_panel & PANEL_DEBUG)
	{
		write({ 1,2 }, L"PANEL_DEBUG");
	}
	if (t_panel & PANEL_CPU_INFO)
	{
		write({ 1,3 }, L"PANEL_CPU_INFO");
	}
	if (t_panel & PANEL_MEMORY_INFO)
	{
		write({ 1,4 }, L"PANEL_MEMORY_INFO");
	}
	if (t_panel & PANEL_LOG_OUTPUT)
	{
		write({ 1,5 }, L"PANEL_LOG_OUTPUT");
	}
	if (t_panel & PANEL_CLIENT_LIST)
	{
		write({ 1,6 }, L"PANEL_CLIENT_LIST");
	}
	if (t_panel & PANEL_COMMAND)
	{
		write({ 1,7 }, L"PANEL_COMMAND");
	}
}

static bool pending = false;
unsigned __stdcall RenderThread(PVOID arg)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	COORD oldSize = g_consoleSize;
	//test code
	initialize_buffer();
	set_input_position({ 4,static_cast<SHORT>(oldSize.Y - 2) });
	set_cursor_position({ 4,static_cast<SHORT>(oldSize.Y - 2) });
	//render_panel({ 0,0 }, { 80,2 });
	//render_info();
	present();
	while (true)
	{
		oldSize = g_consoleSize;
		csbi = get_csbi();
		COORD newSize
		{
			SHORT(csbi.srWindow.Right - csbi.srWindow.Left + 1),
			SHORT(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
		};

		if (newSize.X != oldSize.X || newSize.Y != oldSize.Y)
			set_console_size(newSize);

		set_info_box_size(27 + (newSize.X / 1.3 - MIN_WIDTH));
		clear_console();
		if (newSize.X < MIN_WIDTH || newSize.Y < MIN_HEIGHT)
		{
			write({ 0,0 }, L"Please increase the window size.");
		}
		else
		{
			render_panel();
#pragma region Test Sample
			/*std::wstring title = L"AkaNetCore Server Dashboard v0.5.3-dev";
			write({ static_cast<SHORT>(g_consoleSize.X / 2 - title.size() / 2),1 },
				title, (WORD)FOREGROUND_INTENSITY);
			write({ 2,2 }, L"Uptime: 01:23:45");
			std::wstring date = L"Date: 2026-06-30 20:41:31";
			write({ static_cast<SHORT>(g_consoleSize.X - date.size() - 2),2 },
				date);
			write({ 2,static_cast<SHORT>(g_consoleSize.Y - 2) },
				L">");
			write({ 2,5 }, L"VERSION");
			write({ 20,5 }, L"BUILD DATE");
			write({ 44,5 }, L"PORT");
			write({ 62,5 }, L"USE TLS");
			write({ 80,5 }, L"RUNNING TIME");
			write({ 98,5 }, L"DOMAIN");
			write({ 116,5 }, L"SERVER NAME");
			write({ 2,6 }, L"1.0.0");
			write({ 20,6 }, L"2026-07-01 13:04:23");
			write({ 44,6 }, L"8080");
			write({ 62,6 }, L"O");
			write({ 80,6 }, L"00:01:23");
			write({ 98,6 }, L"Akadals.com");
			write({ 116,6 }, L"Blockia-Dedicated");
			write({ 2,9 }, L"Total:");
			write({ 27,9 }, L"31.8 GiB");
			write({ 2,10 }, L"Used:");
			write({ 27,10 }, L"0.72 GiB");
			write({ 2,11 }, L"▁▁▂▂▃▅▆▇█▇▇▆▃▂▇▇█▂▆▆▃▅▆██▆▇█▇▇▂▂▃",
				(WORD)(FOREGROUND_GREEN));
			write({ 2,12 }, L"Available:");
			write({ 26,12 }, L"30.08 GiB");
			write({ 2,13 }, L"▁▁▂▂▃▅▆▇█▇▇▆▃▂▇▇█▂▆▆▃▅▆██▆▇█▇▇▂▂▃",
				(WORD)(FOREGROUND_GREEN | FOREGROUND_RED));
			write({ 2,16 }, L"Total:");
			write({ 31,16 }, L"214%");
			write({ 2,17 }, L"Core 1:   13%");
			write({ 2,18 }, L"Core 2:    8%");
			write({ 2,19 }, L"Core 3:    1%");
			write({ 2,20 }, L"Core 4:   54%");
			write({ 22,17 }, L"Core 5:   70%");
			write({ 22,18 }, L"Core 6:    2%");
			write({ 22,19 }, L"Core 7:   14%");
			write({ 22,20 }, L"Core 8:   13%");*/
#pragma endregion

		}
		//write({ 1,1 }, std::to_wstring(newSize.X) + L" " + std::to_wstring(newSize.Y));
		present();
	}
	return 0;
}