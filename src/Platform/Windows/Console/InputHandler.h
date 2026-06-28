#pragma once
#include <string>
#include <conio.h>
#include <../include/AkaNetCore/CommandManager.h>

inline std::string g_sInputBuffer;
inline COORD g_pInputPosition;

void SetInputPosition(SHORT x, SHORT y);
void GetInput();
