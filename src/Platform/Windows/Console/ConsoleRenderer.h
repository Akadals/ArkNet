#ifndef _AKACONRENDER_
#define _AKACONRENDER_
#endif

#include "ConsoleBuffer.h"
#include "InputHandler.h"
#include <unordered_map>
#include <array>

#define PANEL_TITLE				0
#define PANEL_SERVER_STATUS		1
#define PANEL_MEMORY			2
#define PANEL_CPU				3
#define PANEL_DATABASE			4
#define PANEL_PROCESS			5
#define PANEL_CLIENT_LIST		6
#define PANEL_THREAD			7
#define PANEL_JOB_QUEUE			8
#define PANEL_PERFORMANCE		9
#define PANEL_CONNECTION		10
#define PANEL_NETWORK			11
#define PANEL_PACKET_CAPTURE	12
#define PANEL_SERVER_LOG		13
#define PANEL_COMMAND_LOG		14
#define PANEL_COMMAND			15

typedef enum class PanelLink : uint8_t
{
	None	= 0,
	Top		= 1 << 0,
	Bottom	= 1 << 1,
	Left	= 1 << 2,
	Right	= 1 << 3,
	ALL		= 0x0F
} PANEL_LINK;
inline constexpr PanelLink operator|(PanelLink a, PanelLink b);

typedef struct PanelAttribute
{
	PANEL_LINK panelLink = PanelLink::None;
	SHORT panelScrollstartOffset = 0;
	WORD panelColorAttribute = DEFAULT_ATTRIBUTE;
	WORD titleColorAttribute = DEFAULT_ATTRIBUTE;

	PanelAttribute(
		WORD t_titleColor = DEFAULT_ATTRIBUTE,
		WORD t_panelColor = DEFAULT_ATTRIBUTE,
		SHORT t_scrollstartOffset = 0,
		PANEL_LINK t_link = PanelLink::None) :
		titleColorAttribute(t_titleColor),
		panelColorAttribute(t_panelColor),
		panelScrollstartOffset(t_scrollstartOffset),
		panelLink(t_link) {}

	PanelAttribute(PANEL_LINK t_link) :
		panelLink(t_link) {}
} PANEL_ATTRIBUTE;
typedef struct PanelPosition
{
	COORD startPosition = {};
	COORD panelScale = {};
	SHORT titlePosition = 0;

	PanelPosition() = default;
	PanelPosition(
		COORD t_start,
		COORD t_scale,
		SHORT t_title = 0) :
		startPosition(t_start),
		panelScale(t_scale),
		titlePosition(t_title) {}
} PANEL_POSITION;
typedef struct PanelViewport
{
	SMALL_RECT screenRect = {};
	SMALL_RECT scrollRect = {};
	SHORT scrollOffset = 0;

	PanelViewport() = default;
	PanelViewport(
		SMALL_RECT t_rect,
		SMALL_RECT t_scrollRect) :
		screenRect(t_rect),
		scrollRect(t_scrollRect) {}
} PANEL_VIEWPORT;

PANEL_VIEWPORT render_panel(
	PANEL_POSITION t_position,
	PANEL_ATTRIBUTE t_attribute = {},
	std::wstring t_title = L"");

void drow_panel();

inline std::unordered_map<uint8_t, PANEL_VIEWPORT> panel;
void write(
	COORD t_position,
	PANEL_VIEWPORT t_viewport,
	std::wstring_view t_text,
	WORD t_attribute = DEFAULT_ATTRIBUTE);

unsigned __stdcall RenderThread(PVOID arg);