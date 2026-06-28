#pragma once
#include <windows.h>
#include <algorithm>
#include <string>
#include <vector>
#include <atomic>

constexpr SHORT MIN_WIDTH = 60;
constexpr SHORT MIN_HEIGHT = 10;

void initialize_buffer();
void close_buffer();

void clear_console(size_t t_consoleIndex);
void clear_console();
void present();

void set_console_size(COORD t_size);

void write(COORD t_position, std::wstring_view t_text, size_t t_consoleIndex, WORD t_attribute);
void write(COORD t_position, std::wstring_view t_text, WORD t_attribute);
void write(COORD t_position, std::wstring_view t_text);

COORD get_size();
std::vector<CHAR_INFO>* get_info();

size_t get_index(COORD t_position);

CONSOLE_SCREEN_BUFFER_INFO get_csbi();