#pragma once
#include <windows.h>
#include <algorithm>
#include <string>
#include <vector>
#include <atomic>
#include <cassert>
#include "../../../Logging/Internal/Logger.h"

constexpr SHORT MIN_WIDTH = 100;
constexpr SHORT MIN_HEIGHT = 24;

constexpr WORD DEFAULT_ATTRIBUTE =
FOREGROUND_RED |
FOREGROUND_GREEN |
FOREGROUND_BLUE;

inline COORD g_consoleSize{ 0,0 };

void initialize_buffer();
void close_buffer();

void clear_console(BOOL t_consoleIndex);
void clear_console();
void present();

void set_console_size(COORD t_size);
void set_cursor_position(COORD t_position);

void force_reset_window(HANDLE t_handle, COORD t_size);
COORD write(
	COORD t_position,
	std::wstring_view t_text,
	size_t t_consoleIndex,
	WORD t_attribute,
	SHORT t_lineBreak = 0);
COORD write(
	SHORT x, SHORT y,
	std::wstring_view t_text,
	size_t t_consoleIndex,
	WORD t_attribute,
	SHORT t_lineBreak = 0);
COORD write(
	COORD t_position,
	std::wstring_view t_text,
	WORD t_attribute,
	SHORT t_lineBreak = 0);
COORD write(
	SHORT x, SHORT y,
	std::wstring_view t_text,
	WORD t_attribute,
	SHORT t_lineBreak = 0);
COORD write(
	COORD t_position,
	std::wstring_view t_text,
	SHORT t_lineBreak = 0);
COORD write(
	SHORT x, SHORT y,
	std::wstring_view t_text,
	SHORT t_lineBreak = 0);

std::vector<CHAR_INFO>* get_info();

size_t get_index(COORD t_position);

CONSOLE_SCREEN_BUFFER_INFO get_csbi();