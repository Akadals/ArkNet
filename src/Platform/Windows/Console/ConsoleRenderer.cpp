#include "ConsoleRenderer.h"

constexpr SHORT STATE_BOX_SIZE = 20;

namespace
{
	std::vector<std::wstring> g_frameBuffer;
	static constexpr wchar_t frameChars[] =
	{
		L'¦¡', L'¦¢', L'¦£', L'¦¤',
		L'¦¥', L'¦¦', L'¦§', L'¦¨',
		L'¦©', L'¦ª'
	};

}

void render_frame()
{
	COORD size = get_size();
	int i;
	g_frameBuffer.clear();
	g_frameBuffer.assign(
		size.Y,
		std::wstring(size.X, L' ')
	);

	g_frameBuffer[0][0] = frameChars[2];
	for (i = 1; i < size.X - STATE_BOX_SIZE;)
		g_frameBuffer[0][i++] = frameChars[0];
	g_frameBuffer[0][i++] = frameChars[7];
	for (; i < size.X - 1;)
		g_frameBuffer[0][i++] = frameChars[0];
	g_frameBuffer[0][size.X - 1] = frameChars[3];

	g_frameBuffer[size.Y - 3][0] = frameChars[6];
	for (i = 1; i < size.X - STATE_BOX_SIZE;)
		g_frameBuffer[size.Y - 3][i++] = frameChars[0];
	g_frameBuffer[size.Y - 3][i++] = frameChars[9];
	for (; i < size.X - 1;)
		g_frameBuffer[size.Y - 3][i++] = frameChars[0];
	g_frameBuffer[size.Y - 3][size.X - 1] = frameChars[8];

	g_frameBuffer[size.Y-1][0] = frameChars[5];
	for (i = 1; i < size.X - 1;)
		g_frameBuffer[size.Y - 1][i++] = frameChars[0];
	g_frameBuffer[size.Y - 1][size.X - 1] = frameChars[4];

	for (int h = 0; ++h < size.Y - 1;)
	{
		if (h == size.Y - 3) continue;
		if (h == size.Y - 2)
		{
			g_frameBuffer[h][1] = L' ';
			g_frameBuffer[h][2] = L'>';
			g_frameBuffer[h][0] =
				g_frameBuffer[h][size.X - 1] = frameChars[1];
			continue;
		}
		g_frameBuffer[h][0] =
			g_frameBuffer[h][size.X - STATE_BOX_SIZE] =
			g_frameBuffer[h][size.X - 1] = frameChars[1];
	}

	for (SHORT i = 0; i < size.Y; i++)
		write({ 0,i }, g_frameBuffer[i]);
}

void render_state()
{
	COORD size = get_size();
	write({ static_cast<SHORT>(size.X - STATE_BOX_SIZE / 2 - 4),1 },
		L"[STATUS]");
}


static bool pending = false;
unsigned __stdcall RenderThread(PVOID arg)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	COORD oldSize{};
	//test code
	initialize_buffer();
	render_frame();
	render_state();
	present();
	while (true)
	{
		oldSize = get_size();
		csbi = get_csbi();
		COORD newSize
		{
			SHORT(csbi.srWindow.Right - csbi.srWindow.Left + 1),
			SHORT(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
		};
		set_console_size(newSize);
		if (newSize.X < MIN_WIDTH || newSize.Y < MIN_HEIGHT)
		{
			clear_console();
			write({ 0,0 }, L"Please increase the window size.");
		}
		else
		{
			render_frame();
			render_state();
		}
		present();
	}
	return 0;
}