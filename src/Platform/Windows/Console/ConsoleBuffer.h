#pragma once
#define WIN32_LEAN_AND_MEAN

#define MIN_WIDTH 100
#define MIN_HEIGHT 24

#define DEFAULT_ATTRIBUTE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

#define MAKECOORD(x,y) COORD{ static_cast<SHORT>(x),static_cast<SHORT>(y) }
#define MAKERECT(left,top,right,bottom) \
SMALL_RECT{ static_cast<SHORT>(left),static_cast<SHORT>(top),\
static_cast<SHORT>(right),static_cast<SHORT>(bottom) }
#define MAKEASTR(text,attribute) ASTRING(text,attribute)

#include <windows.h>
#include <algorithm>
#include <string>
#include <vector>
#include <atomic>
#include <cassert>
#include <mutex>
#include "../../../Logging/Internal/Logger.h"


inline std::mutex mtx;
inline COORD g_consoleSize{ 0,0 };


void initialize_buffer();
void close_buffer();

void clear_console(BOOL t_consoleIndex);
void clear_console();
void present();

void set_console_size(COORD t_size);
void set_cursor_position(COORD t_position);

void force_reset_window(HANDLE t_handle, COORD t_size);

typedef struct AttributedString
{
	std::wstring_view string;
	WORD attribute;

	AttributedString(
		std::wstring_view t_text,
		WORD t_attribute = DEFAULT_ATTRIBUTE) :
		string(t_text),
		attribute(t_attribute) {}
} ATTRIBUTED_STRING, ASTRING;

typedef struct AttributedText
{
	std::vector<ASTRING> strings;

	[[nodiscard]]
	size_t size() const noexcept
	{
		size_t size = 0;
		for (const ASTRING& str : strings)
		{
			size += str.string.size();
		}
		return size;
	}

	AttributedText() = default;
	AttributedText(std::wstring_view t_text)
	{
		strings.emplace_back(ASTRING{ t_text, DEFAULT_ATTRIBUTE });
	}
	AttributedText(const wchar_t* t_text)
	{
		strings.emplace_back(ASTRING{ t_text, DEFAULT_ATTRIBUTE });
	}
	AttributedText(ASTRING t_astring)
	{
		strings.emplace_back(t_astring);
	}
} ATTRIBUTED_TEXT, ATEXT;

ATEXT operator+(ASTRING lhs, ASTRING rhs);
ATEXT operator+(ATEXT lhs, ASTRING rhs);
ATEXT operator+(ATEXT lhs, std::wstring_view rhs);
ATEXT operator+(ASTRING lhs, std::wstring_view rhs);
ATEXT operator+(std::wstring_view lhs, ASTRING rhs);
ATEXT operator+(std::wstring_view lhs, std::wstring_view rhs);
ATEXT operator+(const wchar_t* lhs, ASTRING rhs);
ATEXT operator+(ASTRING lhs, const wchar_t* rhs);

COORD write(
	COORD t_position,
	ATEXT t_text,
	size_t t_consoleIndex,
	SHORT t_lineBreak = 0);
COORD write(
	COORD t_position,
	ATEXT t_text,
	SHORT t_lineBreak = 0);

size_t get_index(COORD t_position);

CONSOLE_SCREEN_BUFFER_INFO get_csbi();