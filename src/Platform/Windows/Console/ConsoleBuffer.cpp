#include "ConsoleBuffer.h"

namespace
{
	constexpr size_t CONSOLE_COUNT = 2;

	struct ScreenBuffer
	{
		HANDLE handle{};
		std::vector<CHAR_INFO> cells;
	};

	ScreenBuffer	g_console[CONSOLE_COUNT];
	size_t			g_activeConsole = 0;
	COORD			g_consoleSize{ 0,0 };
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
	ERROR_LOG(SetConsoleMode(g_console[0].handle, mode));

	ERROR_LOG(GetConsoleMode(g_console[1].handle, &mode));
	mode &= ~ENABLE_WRAP_AT_EOL_OUTPUT;
	mode &= ~ENABLE_PROCESSED_OUTPUT;
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

void clear_console(size_t t_consoleIndex)
{
	auto& buffer = g_console[t_consoleIndex];

	DWORD written = 0;

	for (CHAR_INFO& cell : buffer.cells)
	{
		cell.Char.UnicodeChar = L' ';
		cell.Attributes = DEFAULT_ATTRIBUTE;
	}
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
		&rect));

	ERROR_LOG(SetConsoleActiveScreenBuffer(g_console[g_activeConsole].handle));
	g_activeConsole ^= 1;

	clear_console();
}

void set_console_size(COORD t_size)
{
	if (t_size.X == g_consoleSize.X &&
		t_size.Y == g_consoleSize.Y)
		return;

	SMALL_RECT rect
	{
		0,
		0,
		static_cast<SHORT>(t_size.X - 1),
		static_cast<SHORT>(t_size.Y - 1)
	};

	for (auto& console : g_console)
	{
		ERROR_LOG(SetConsoleCursorPosition(console.handle, { 0,0 }));
		if ((t_size.X > g_consoleSize.X) || (t_size.Y > g_consoleSize.Y))
		{
			ERROR_LOG(SetConsoleScreenBufferSize(console.handle, t_size));
			ERROR_LOG(SetConsoleWindowInfo(console.handle, TRUE, &rect));
		}
		else
		{
			ERROR_LOG(SetConsoleWindowInfo(console.handle, TRUE, &rect));
			ERROR_LOG(SetConsoleScreenBufferSize(console.handle, t_size));
		}
	}

	g_consoleSize = t_size;

	const size_t cellCount =
		static_cast<size_t>(t_size.X) *
		static_cast<size_t>(t_size.Y);

	for (auto& console : g_console)
		std::vector<CHAR_INFO>(cellCount, CHAR_INFO{
			{ L' ' },
			DEFAULT_ATTRIBUTE
			}).swap(console.cells);
	return;
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

	if (t_consoleIndex >= CONSOLE_COUNT)
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
	auto words = split(std::wstring(t_text), L' ');

	for (size_t i = 0; i < words.size(); ++i)
	{
		const std::wstring& word = words[i];

		if (t_lineBreak > 0 &&
			areaSize > 0 &&
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
			if (t_lineBreak > 0)
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
	return { static_cast<SHORT>(t_text.size()),lineBreakCount };
}

COORD write(
	COORD t_position,
	std::wstring_view t_text, 
	WORD t_attribute, 
	SHORT t_lineBreak)
{
	return write(t_position, t_text, g_activeConsole, t_attribute, t_lineBreak);
}

COORD write(
	COORD t_position,
	std::wstring_view t_text, 
	SHORT t_lineBreak)
{
	return write(t_position, t_text, g_activeConsole, DEFAULT_ATTRIBUTE, t_lineBreak);
}

COORD get_size()
{
	return g_consoleSize;
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
