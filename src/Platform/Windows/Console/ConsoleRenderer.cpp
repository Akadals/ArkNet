#include "ConsoleRenderer.h"

namespace
{
	static constexpr wchar_t panelFrames[] =
	{
		L'─', L'│', L'╭', L'╮',
		L'╯', L'╰', L'├', L'┬',
		L'┤', L'┴',L'┼'
	};
	static constexpr wchar_t usageBars[] =
	{
		L'▁',L'▂',L'▃',L'▄',L'▅',L'▆',L'▇',L'█'
	};
	constexpr std::array<std::array<wchar_t, 4>, 16> panelConnector
	{ {
			{ L'╭', L'╮', L'╰', L'╯' },
			{ L'├', L'┤', L'╰', L'╯' },
			{ L'╭', L'╮', L'├', L'┤' },
			{ L'├', L'┤', L'├', L'┤' },
			{ L'┬', L'╮', L'┴', L'╯' },
			{ L'┼', L'┤', L'┴', L'╯' },
			{ L'┬', L'╮', L'┼', L'┤' },
			{ L'┼', L'┤', L'┼', L'┤' },
			{ L'╭', L'┬', L'╰', L'┴' },
			{ L'├', L'┼', L'╰', L'┴' },
			{ L'╭', L'┬', L'├', L'┼' },
			{ L'├', L'┼', L'├', L'┼' },
			{ L'┬', L'┬', L'┴', L'┴' },
			{ L'┼', L'┼', L'┴', L'┴' },
			{ L'┬', L'┬', L'┼', L'┼' },
			{ L'┼', L'┼', L'┼', L'┼' },
		} };
}

inline constexpr PanelLink operator|(PanelLink a, PanelLink b)
{
	return static_cast<PanelLink>(
		static_cast<uint8_t>(a) |
		static_cast<uint8_t>(b)
		);
}

PANEL_VIEWPORT render_panel(
	PANEL_POSITION t_position,
	PANEL_ATTRIBUTE t_attribute,
	std::wstring t_title)
{
	SHORT i = 0;

	SMALL_RECT viewportRect =
		MAKERECT(
			t_position.startPosition.X + 2,
			t_position.startPosition.Y + 1,
			t_position.startPosition.X +
			t_position.panelScale.X - 1,
			t_position.startPosition.Y +
			t_position.panelScale.Y);
	PanelViewport viewport =
	{
		viewportRect,
		MAKERECT(
			viewportRect.Left,
			viewportRect.Top + 
			t_attribute.panelScrollstartOffset,
			viewportRect.Right,
			viewportRect.Bottom)
	};

	std::array<wchar_t, 4> corner =
		panelConnector[static_cast<uint8_t>(t_attribute.panelLink)];

	write(
		t_position.startPosition,
		{ &corner[0],1 },
		t_attribute.panelColorAttribute);
	write(
		MAKECOORD(
			t_position.startPosition.X + 
			t_position.panelScale.X + 1, 
			t_position.startPosition.Y),
		{ &corner[1],1 },
		t_attribute.panelColorAttribute);
	write(
		MAKECOORD(
			t_position.startPosition.X, 
			t_position.startPosition.Y + 
			t_position.panelScale.Y + 1),
		{ &corner[2],1 },
		t_attribute.panelColorAttribute);
	write(
		MAKECOORD(
			t_position.startPosition.X +
			t_position.panelScale.X + 1,
			t_position.startPosition.Y +
			t_position.panelScale.Y + 1),
		{ &corner[3],1 },
		t_attribute.panelColorAttribute);

	for (i = 1; i < t_position.panelScale.X + 1;)
		write(
			MAKECOORD(
				t_position.startPosition.X + i++,
				t_position.startPosition.Y),
			{ &panelFrames[0],1 },
			t_attribute.panelColorAttribute);
	for (i = 1; i < t_position.panelScale.Y + 1;)
	{
		write(
			MAKECOORD(
				t_position.startPosition.X,
				t_position.startPosition.Y + i),
			{ &panelFrames[1],1 },
			t_attribute.panelColorAttribute);
		write(
			MAKECOORD(
				t_position.startPosition.X +
				t_position.panelScale.X + 1,
				t_position.startPosition.Y + i++),
			{ &panelFrames[1],1 },
			t_attribute.panelColorAttribute);
	}
	for (i = 1; i < t_position.panelScale.X + 1;)
		write(
			MAKECOORD(
				t_position.startPosition.X + i++,
				t_position.startPosition.Y +
				t_position.panelScale.Y + 1),
			{ &panelFrames[0],1 },
			t_attribute.panelColorAttribute);

	if (!t_title.empty())
	{
		t_title = L" " + t_title + L" ";
		write(
			MAKECOORD(
				t_position.startPosition.X +
				t_position.titlePosition + 1,
				t_position.startPosition.Y),
			t_title,
			t_attribute.titleColorAttribute);
	}

	return viewport;
}
void drow_panel()
{
#pragma region TOP
	panel[PANEL_TITLE] = 
		render_panel(
		{
			MAKECOORD(0, 0),
			MAKECOORD(g_consoleSize.X - 2, 1)
		});
	panel[PANEL_SERVER_STATUS] = 
		render_panel(
		{
			MAKECOORD(0, 2),
			MAKECOORD(g_consoleSize.X - 2, 1)
		},
		{ PanelLink::Top },
		L"");
#pragma endregion
#pragma region SYSTEM
	render_panel(
		{
			MAKECOORD(0, 5),
			MAKECOORD(40, g_consoleSize.Y - 10),
			3
		},
		{
			FOREGROUND_BLUE |
			FOREGROUND_INTENSITY
		},
		L"[ SYSTEM ]");
	panel[PANEL_MEMORY] =
		render_panel(
			{
				MAKECOORD(1, 6),
				MAKECOORD(38, 6),
				3
			},
			{
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			},
			L"Memory");
	SHORT height = get_cpu_core_count() / 2;
	panel[PANEL_CPU] =
		render_panel(
			{
				MAKECOORD(1, 14),
				MAKECOORD(38, height + 4),
				3
			},
			{
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			},
			L"CPU");
	panel[PANEL_DATABASE] =
		render_panel(
			{
				MAKECOORD(1, height + 20),
				MAKECOORD(38, 10),
				3
			},
			{
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			},
			L"Database");
	panel[PANEL_PROCESS] =
		render_panel(
			{
				MAKECOORD(1, height + 32),
				MAKECOORD(38, g_consoleSize.Y - 46),
				3
			},
			{
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			},
			L"Process");
#pragma endregion
#pragma region SERVER
	render_panel(
		{
			MAKECOORD(42, 5),
			MAKECOORD(95, g_consoleSize.Y - 10),
			3
		},
		{
			FOREGROUND_RED |
			FOREGROUND_BLUE |
			FOREGROUND_INTENSITY
		},
		L"[ SERVER ]");
	panel[PANEL_CLIENT_LIST] =
		render_panel(
			{
				MAKECOORD(43, 6),
				MAKECOORD(93, 10),
				3
			},
			{
				FOREGROUND_RED |
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY,
				DEFAULT_ATTRIBUTE,
				1
			},
			L"Client List (0 / 0) (F1)");
	panel[PANEL_THREAD] =
		render_panel(
			{
				MAKECOORD(43, 18),
				MAKECOORD(93, 10),
				3
			},
			{
				FOREGROUND_RED |
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY,
				DEFAULT_ATTRIBUTE,
				1
			},
			L"Thread (0) (F2)");
	panel[PANEL_JOB_QUEUE] =
		render_panel(
			{
				MAKECOORD(43, 30),
				MAKECOORD(35, 7),
				3
			},
			{
				FOREGROUND_RED |
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			},
			L"Job Queue");
	panel[PANEL_PERFORMANCE] =
		render_panel(
			{
				MAKECOORD(80, 30),
				MAKECOORD(56, 7),
				3
			},
			{
				FOREGROUND_RED |
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			},
			L"Performance");
	panel[PANEL_CONNECTION] =
		render_panel(
			{
				MAKECOORD(43, 39),
				MAKECOORD(93, g_consoleSize.Y - 45),
				3
			},
			{
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			},
			L"Connection");
#pragma endregion
#pragma region NETWORK
	render_panel(
		{
			MAKECOORD(139, 5),
			MAKECOORD(g_consoleSize.X - 141, 20),
			3
		},
		{
			FOREGROUND_BLUE
		},
		L"[ NETWORK ]");
	panel[PANEL_NETWORK] =
		render_panel(
			{
				MAKECOORD(140, 6),
				MAKECOORD(g_consoleSize.X - 143, 8),
				3
			},
			{
				FOREGROUND_BLUE
			},
			L"Network");
	panel[PANEL_PACKET_CAPTURE] =
		render_panel(
			{
				MAKECOORD(140, 16),
				MAKECOORD(g_consoleSize.X - 143, 8),
				3
			},
			{
				FOREGROUND_BLUE,
				DEFAULT_ATTRIBUTE,
				1
			},
			L"Packet Capture (Last 50) (F3)");
#pragma endregion
#pragma region LOG
	render_panel(
		{
			MAKECOORD(139, 27),
			MAKECOORD(g_consoleSize.X - 141, g_consoleSize.Y - 29),
			3
		},
		{
			FOREGROUND_GREEN |
			FOREGROUND_INTENSITY
		},
		L"[ LOG ]");
	panel[PANEL_SERVER_LOG] =
		render_panel(
			{
				MAKECOORD(140, 28),
				MAKECOORD(g_consoleSize.X - 143, 9),
				3
			},
			{
				FOREGROUND_GREEN |
				FOREGROUND_INTENSITY,
			},
			L"Server Log (Last 100) (F4)");
	panel[PANEL_COMMAND_LOG] =
		render_panel(
			{
				MAKECOORD(140, 39),
				MAKECOORD(g_consoleSize.X - 143, 9),
				3
			},
			{
				FOREGROUND_GREEN |
				FOREGROUND_INTENSITY
			},
			L"Command Log (F5)");
#pragma endregion
#pragma region BOTTOM
	panel[PANEL_COMMAND] =
		render_panel(
			{
				MAKECOORD(0, g_consoleSize.Y - 3),
				MAKECOORD(137, 1),
				3
			},
			{},
			L"COMMAND");
#pragma endregion
}

void write(
	COORD t_position, 
	PANEL_VIEWPORT t_viewport,
	std::wstring_view t_text,
	WORD t_attribute)
{
	COORD absolutePosition =
	{
		t_viewport.screenRect.Left +
		t_position.X,
		t_viewport.screenRect.Top +
		t_position.Y
	};
	write(
		absolutePosition,
		t_text,
		t_attribute,
		t_viewport.screenRect.Right -
		t_viewport.screenRect.Left + 1);
}

unsigned __stdcall RenderThread(PVOID arg)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	COORD oldSize = g_consoleSize;
	initialize_buffer();
	set_input_position({ 4,static_cast<SHORT>(oldSize.Y - 2) });
	set_cursor_position({ 4,static_cast<SHORT>(oldSize.Y - 2) });
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

		clear_console();
		if (newSize.X < MIN_WIDTH || newSize.Y < MIN_HEIGHT)
		{
			write({ 0,0 }, L"Please increase the window size.");
		}
		else
		{
			drow_panel();
		}
		present();
	}
	return 0;
}