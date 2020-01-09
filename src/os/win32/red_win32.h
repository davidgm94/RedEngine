#pragma once
#if _WIN64
#define _CRT_SECURE_NO_WARNINGS 1
#include "../red_os.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define QPC(name) i64 name; (void)QueryPerformanceCounter((LARGE_INTEGER*)&name)
typedef struct
{
	HINSTANCE instance;
	HINSTANCE previousInstance;
	PSTR commandLineArguments;
	int showCommand;
} winmain_parameters;
#endif
typedef struct
{
	winmain_parameters winmainArguments;
	HWND window;
} win32_native_handles;
typedef struct
{
	bool s[4];
} win32_application_state;
typedef struct
{
	i64 performanceFrequency;
	i64 firstPerformanceCounter;
} win32_timer;
typedef struct
{
	win32_native_handles handles;
	win32_application_state applicationState;
	win32_timer timer;
} win32_window_application;

static win32_window_application win32;

#define CompletePastWritesBeforeFutureWrites _WriteBarrier(); _mm_sfence()
#define CompletePastReadsBeforeFutureReads _ReadBarrier()

win32_window_application* win32_startup(const char* windowTitle, os_window_dimensions* window, win32_window_application* win32);

