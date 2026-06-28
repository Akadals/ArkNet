#pragma once
#include "ConsoleBuffer.h"
#include "InputHandler.h"
#include <vector>
#include <string>

void render_frame();
void render_state();
unsigned __stdcall RenderThread(PVOID arg);