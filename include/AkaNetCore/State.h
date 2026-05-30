#pragma once

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
	
	RunningState GetThreadState(HANDLE handle);
}
