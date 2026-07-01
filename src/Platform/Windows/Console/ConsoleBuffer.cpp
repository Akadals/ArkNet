#include "ConsoleBuffer.h"

namespace
{
	struct ScreenBuffer
	{
		HANDLE handle{};
		std::vector<CHAR_INFO> cells;
	};

	ScreenBuffer	g_console[2];
	BOOL			g_activeConsole = 0;
}

void initialize_buffer()
{
	g_console[0].handle =
		CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CONSOLE_TEXTMODE_BUFFER,
			NULL);
	g_console[1].handle =
		CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CONSOLE_TEXTMODE_BUFFER,
			NULL);

	DWORD mode;

	ERROR_LOG(GetConsoleMode(g_console[0].handle, &mode));
	mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
	mode &= ~ENABLE_PROCESSED_OUTPUT;
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	ERROR_LOG(SetConsoleMode(g_console[0].handle, mode));
	ERROR_LOG(SetConsoleMode(g_console[1].handle, mode));

	CONSOLE_CURSOR_INFO ci{};
	ci.dwSize = 1;
	ci.bVisible = FALSE;

	ERROR_LOG(SetConsoleCursorInfo(g_console[0].handle, &ci));
	ERROR_LOG(SetConsoleCursorInfo(g_console[1].handle, &ci));

	if (g_console[0].handle == INVALID_HANDLE_VALUE ||
		g_console[1].handle == INVALID_HANDLE_VALUE)
	{
		internalLog += "INVALID_HANDLE_VALUE";
		return;
	}

	ERROR_LOG(SetConsoleActiveScreenBuffer(g_console[0].handle));

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	ERROR_LOG(GetConsoleScreenBufferInfo(g_console[0].handle, &csbi));

	g_consoleSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	g_consoleSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	for (auto& console : g_console)
		console.cells.resize(
			static_cast<size_t>(g_consoleSize.X) *
			g_consoleSize.Y);
}

void close_buffer()
{
	for (auto& console : g_console)
	{
		if (console.handle != INVALID_HANDLE_VALUE)
		{
			ERROR_LOG(CloseHandle(console.handle));
			console.handle = INVALID_HANDLE_VALUE;
		}
		console.cells.clear();
	}
}

void clear_console(BOOL t_consoleIndex)
{
	auto& buffer = g_console[t_consoleIndex];

	DWORD written = 0;

	std::fill(
		buffer.cells.begin(),
		buffer.cells.end(),
		CHAR_INFO{ {L' '}, DEFAULT_ATTRIBUTE }
	);
}

void clear_console()
{
	clear_console(g_activeConsole);
}

void present()
{
	SMALL_RECT rect
	{
		0,
		0,
		static_cast<SHORT>(g_consoleSize.X - 1),
		static_cast<SHORT>(g_consoleSize.Y - 1)
	};
	ERROR_LOG(WriteConsoleOutputW(
		g_console[g_activeConsole].handle,
		g_console[g_activeConsole].cells.data(),
		g_consoleSize,
		{ 0, 0 },
		& rect));

	ERROR_LOG(SetConsoleActiveScreenBuffer(g_console[g_activeConsole].handle));
	g_activeConsole ^= 1;
}
void set_console_size(COORD t_size)
{
	g_consoleSize = t_size;
	const size_t cellCount =
		static_cast<size_t>(t_size.X) *
		static_cast<size_t>(t_size.Y);

	for (auto& console : g_console)
	{
		force_reset_window(console.handle, t_size);

		std::vector<CHAR_INFO>(cellCount, CHAR_INFO{
			{ L' ' },
			DEFAULT_ATTRIBUTE
			}).swap(console.cells);
	}
	return;
}

void set_cursor_position(COORD t_position)
{
	ERROR_LOG(SetConsoleCursorPosition(g_console[0].handle, t_position));
	ERROR_LOG(SetConsoleCursorPosition(g_console[1].handle, t_position));
}
std::vector<SMALL_RECT> debug;
void force_reset_window(HANDLE t_handle, COORD t_size)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	ERROR_LOG(GetConsoleScreenBufferInfo(t_handle, &csbi));
	debug.push_back(csbi.srWindow);
	SMALL_RECT rect =
	{
		0,
		0,
		t_size.X - 1,
		t_size.Y - 1
	};
	ERROR_LOG(SetConsoleScreenBufferSize(t_handle, t_size));
	ERROR_LOG(SetConsoleWindowInfo(t_handle, TRUE, &rect));
	ERROR_LOG(SetConsoleCursorPosition(t_handle, { 0,0 }));
}
COORD write(
	COORD t_position,
	std::wstring_view t_text,
	size_t t_consoleIndex,
	WORD t_attribute,
	SHORT t_lineBreak)
{
	size_t written = 0;
	size_t index = get_index(t_position);

	if (t_consoleIndex >= 2)
		return {};

	if (t_position.X < 0 ||
		t_position.Y < 0 ||
		t_position.X >= g_consoleSize.X ||
		t_position.Y >= g_consoleSize.Y)
		return {};

	auto& buffer = g_console[t_consoleIndex];

	int areaSize = 0;
	SHORT lineBreakCount = 0;
	bool pendingLine = false;

	COORD position = t_position;

	if (t_lineBreak > 0)
	{
		auto words = split(std::wstring(t_text), L' ');

		for (size_t i = 0; i < words.size(); ++i)
		{
			const std::wstring& word = words[i];

			if (areaSize > 0 &&
				areaSize + word.size() > t_lineBreak)
			{
				position.X = t_position.X;
				position.Y++;

				if (position.Y >= g_consoleSize.Y)
					break;

				index = get_index(position);
				areaSize = 0;
				lineBreakCount++;
			}
			for (wchar_t ch : word)
			{
				if (position.X >= g_consoleSize.X)
					break;

				buffer.cells[index].Char.UnicodeChar = ch;
				buffer.cells[index].Attributes = t_attribute;

				++index;
				++position.X;
				++areaSize;
				++written;
			}

			if (i + 1 < words.size())
			{
				if (areaSize + 1 > t_lineBreak)
				{
					position.X = t_position.X;
					position.Y++;

					if (position.Y >= g_consoleSize.Y)
						break;

					index = get_index(position);
					areaSize = 0;
					lineBreakCount++;
				}
				else
				{
					buffer.cells[index].Char.UnicodeChar = L' ';
					buffer.cells[index].Attributes = t_attribute;

					++index;
					++position.X;
					++areaSize;
					++written;
				}
			}
		}
	}
	else
	{
		for (wchar_t ch : std::wstring(t_text))
		{
			if (position.X >= g_consoleSize.X)
				break;

			buffer.cells[index].Char.UnicodeChar = ch;
			buffer.cells[index].Attributes = t_attribute;

			++index;
			++position.X;
			++areaSize;
			++written;
		}
	}
	return { static_cast<SHORT>(t_text.size()),lineBreakCount };
}

COORD write(
	SHORT x,
	SHORT y,
	std::wstring_view t_text, 
	size_t t_consoleIndex, 
	WORD t_attribute, 
	SHORT t_lineBreak)
{
	return write(
		{ x,y },
		t_text,
		t_consoleIndex,
		t_attribute,
		t_lineBreak);
}

COORD write(
	COORD t_position,
	std::wstring_view t_text, 
	WORD t_attribute, 
	SHORT t_lineBreak)
{
	return write(
		t_position, 
		t_text, 
		g_activeConsole, 
		t_attribute, 
		t_lineBreak);
}

COORD write(
	SHORT x,
	SHORT y,
	std::wstring_view t_text, 
	WORD t_attribute, 
	SHORT t_lineBreak)
{
	return write(
		{ x,y }, 
		t_text, 
		g_activeConsole, 
		t_attribute, 
		t_lineBreak);
}

COORD write(
	COORD t_position,
	std::wstring_view t_text, 
	SHORT t_lineBreak)
{
	return write(
		t_position,
		t_text, 
		g_activeConsole,
		DEFAULT_ATTRIBUTE,
		t_lineBreak);
}

COORD write(
	SHORT x,
	SHORT y,
	std::wstring_view t_text, 
	SHORT t_lineBreak)
{
	return write(
		{x,y}, 
		t_text,
		g_activeConsole,
		DEFAULT_ATTRIBUTE, 
		t_lineBreak);
}

std::vector<CHAR_INFO>* get_info()
{
	return &g_console[g_activeConsole].cells;
}

size_t get_index(COORD t_position)
{
	return static_cast<size_t>(t_position.Y) *
		g_consoleSize.X + t_position.X;
}

CONSOLE_SCREEN_BUFFER_INFO get_csbi()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	ERROR_LOG(GetConsoleScreenBufferInfo(
		g_console[g_activeConsole].handle, &csbi));

	return csbi;
}
