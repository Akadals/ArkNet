#pragma once
#include "ConsoleBuffer.h"
#include "InputHandler.h"
#include <unordered_map>

#define PANEL_NETWORK 0x0002
#define PANEL_MEMORY_INFO 0x0004
#define PANEL_CPU_INFO	 0x0008
#define PANEL_DEBUG 0x0020
#define PANEL_CLIENT_LIST 0x0040
#define PANEL_LOG_OUTPUT 0x0080
#define PANEL_COMMAND 0x0800
#define PANEL_ALL  PANEL_NETWORK \
				 | PANEL_MEMORY_INFO \
				 | PANEL_CPU_INFO \
				 | PANEL_DEBUG \
				 | PANEL_CLIENT_LIST \
				 | PANEL_LOG_OUTPUT \
				 | PANEL_COMMAND

//inline std::unordered_map<int, PANEL> g_panel;

void set_info_box_size(size_t t_size);

void render_panel(
	COORD t_start,
	COORD t_size,
	std::wstring_view t_title = L"",
	WORD t_attribute = DEFAULT_ATTRIBUTE);
void render_panel(
	COORD t_start,
	SHORT x, SHORT y, 
	std::wstring_view t_title = L"", 
	WORD t_attribute = DEFAULT_ATTRIBUTE);

void render_panel();
void render(int t_panel);
unsigned __stdcall RenderThread(PVOID arg);