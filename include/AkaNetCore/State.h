#pragma once
#include <Windows.h>
namespace AkaNetCore::State
{
	enum RunningState : uint8_t
	{
		RUNNING,
		STOPPED,
	};
	enum SpeedState : uint8_t
	{
		GOOD,
		SLOW,
		BAD,
	};
	
	struct ThreadState
	{
		RunningState running_state;
	};

	typedef struct HandleExpend
	{
		HANDLE handle;
		const char* name = "Unknown";
	} HANDLE_EX, * PHANDLE_EX;

	ThreadState GetThreadState(HANDLE handle);
}
