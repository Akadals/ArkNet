#include "ConsoleRenderer.h"

constexpr SHORT STATE_BOX_SIZE = 30;
constexpr SHORT MIN_STATE_BOX_SIZE = 27;

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
	SHORT i;

	write({ 0,0 }, { &frameChars[2],1 });
	for (i = 1; i < size.X - STATE_BOX_SIZE;)
		write({ i++,0 }, { &frameChars[0],1 });
	write({ i++,0 }, { &frameChars[7],1 });
	for (; i < size.X - 1;)
		write({ i++,0 }, { &frameChars[0],1 });
	write({ i++,0 }, { &frameChars[3],1 });
	write({ 0, static_cast<SHORT>(size.Y - 3) }, 
		{ &frameChars[6],1 });
	for (i = 1; i < size.X - STATE_BOX_SIZE;)
		write({ i++, static_cast<SHORT>(size.Y - 3) }, 
			{ &frameChars[0],1 });
	write({ i++, static_cast<SHORT>(size.Y - 3) }, 
		{ &frameChars[9],1 });
	for (; i < size.X - 1;)
		write({ i++, static_cast<SHORT>(size.Y - 3) }, 
			{ &frameChars[0],1 });
	write({ static_cast<SHORT>(size.X - 1), 
		static_cast<SHORT>(size.Y - 3) }, 
		{ &frameChars[8],1 });
	write({ 0, static_cast<SHORT>(size.Y - 1) }, 
		&frameChars[5]);
	for (i = 1; i < size.X - 1;)
		write({ i++, static_cast<SHORT>(size.Y - 1) }, 
			{ &frameChars[0],1 });
	write({ static_cast<SHORT>(size.X - 1), 
		static_cast<SHORT>(size.Y - 1) }, 
		{ &frameChars[4],1 });


	for (SHORT h = 0; ++h < size.Y - 1;)
	{
		if (h == size.Y - 3) continue;
		if (h == size.Y - 2)
		{
			write({ 1, h }, L" > ");
			write({ 0,h }, { &frameChars[1],1 });
			write({ static_cast<SHORT>(size.X - 1),h }, 
				{ &frameChars[1],1 });
			continue;
		}
		write({ 0,h }, { &frameChars[1],1 });
		write({ static_cast<SHORT>(size.X - STATE_BOX_SIZE),h }, 
			{ &frameChars[1],1 });
		write({ static_cast<SHORT>(size.X - 1),h }, 
			{ &frameChars[1],1 });
	}
}

void render_state()
{
	COORD size = get_size();
	COORD len;
	SHORT start = static_cast<SHORT>(size.X - STATE_BOX_SIZE + 2);

	write({ static_cast<SHORT>(size.X - STATE_BOX_SIZE / 2 - 7),1 },
		L"[HARDWARE INFO]");
	write({ static_cast<SHORT>(size.X - STATE_BOX_SIZE + 2),2 }, L"CPU: ");
	len = write(
		{ static_cast<SHORT>(start + 5),2 },
		get_cpu_name(),
		static_cast<SHORT>(STATE_BOX_SIZE - 9));

	std::wostringstream memSize;
	memSize << std::fixed << std::setprecision(1) << get_memory_size(2);
	std::wstring sizeResult = memSize.str();

	std::wostringstream memUsage;
	memUsage << std::fixed << std::setprecision(1) << get_memory_usage(1);
	std::wstring usageResult = memUsage.str();

	write({ start ,
		static_cast<SHORT>(4 + len.Y) },
		L"Memory Size: ");
	write({ static_cast<SHORT>(start + 13) ,
		static_cast<SHORT>(4 + len.Y) },
		sizeResult + L" MB",
		static_cast<SHORT>(STATE_BOX_SIZE - 17));
	write({ start ,
		static_cast<SHORT>(6 + len.Y) },
		L"Memory Usage: ");
	len = write({ static_cast<SHORT>(start + 14) ,
		static_cast<SHORT>(6 + len.Y) },
		usageResult + L" KB",
		static_cast<SHORT>(STATE_BOX_SIZE - 18));
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