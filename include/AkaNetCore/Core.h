#pragma once

#ifdef _WIN32

#ifdef AKANETCORE_SHARED

#ifdef AKANETCORE_EXPORTS
#define AKA_API __declspec(dllexport)
#else
#define AKA_API __declspec(dllimport)
#endif

#else
#define AKA_API
#endif

#else
#define AKA_API
#endif

#define TO_STRING(x) #x

#define MAX_BUF_SIZE 2048

#define ANCBASEERR	100000

#define ANCERRGOOD				0
#define ANCERRINVALTYPE			(ANCBASEERR + 1)
#define ANCERRINVALINCL			(ANCBASEERR + 2)
#define ANCERRINVALSESH			(ANCBASEERR + 3)
#define ANCERRBADHANDSHAKE		(ANCBASEERR + 4)
#define ANCERRBADCONN			(ANCBASEERR + 5)
#define ANCERRFIREWALLBLOCK		(ANCBASEERR + 6)
#define ANCERRAUTHORITY			(ANCBASEERR + 7)
#define ANCERRBADIP				(ANCBASEERR + 8)
#define ANCERRBADPORT			(ANCBASEERR + 9)
#define ANCERRNOTALLOW			(ANCBASEERR + 10)
#define ANCERRBADREQ			(ANCBASEERR + 11)
#define ANCERRBADTLSCRT			(ANCBASEERR + 12)
#define ANCERRBADTLSKEY			(ANCBASEERR + 13)
#define ANCERRBADTRAFFIC		(ANCBASEERR + 14)
#define ANCERRBADHARDWARE		(ANCBASEERR + 15)

#include <WinSock2.h>
#include <MSWSock.h>

#include <atomic>
#include <string>
#include <mutex>

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

class DWORD_EX
{
public:
	DWORD_EX(DWORD dword) : m_word(dword) {}
public:
	operator DWORD() const { return m_word; }
public:
	std::string ToString() const { return std::to_string(m_word); }
private:
	DWORD m_word;
};

namespace AkaNetCore
{
	inline std::atomic<bool> g_running;
}