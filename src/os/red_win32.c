#if _WIN64
#include "red_win32.h"

#include <stdio.h>

LRESULT CALLBACK win32_message_callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK win32_message_callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	application_state* app_info = ((application_state*)GetWindowLongPtr(window, GWLP_USERDATA));
	switch (message)
	{
	case (WM_PAINT):
	{
		(void)ValidateRect(window, nullptr);
	} break;
	case (WM_CLOSE):
	{
		if (app_info)
		{
			app_info->s[APPLICATION_STATE_RUNNING] = false;
			DestroyWindow(window);
			PostQuitMessage(0);
		}
	} break;
	case (WM_CREATE):
	{
		LPCREATESTRUCT p_create_struct = (LPCREATESTRUCT)(lParam);
		(void)SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)(p_create_struct->lpCreateParams));
	} break;
	default:
	{

	} break;
	}
	return DefWindowProc(window, message, wParam, lParam);
}

static inline void win32_setupConsole(const char* title)
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w+", stdout);
	freopen_s(&stream, "CONOUT$", "w+", stderr);
	SetConsoleTitle(TEXT(title));
}

static inline HWND win32_create_window(HINSTANCE instance, WNDPROC window_proc, int width, int height, const char* window_title, void* pointer_to_data)
{
	WNDCLASSEX window_class = { 0 };
	window_class.cbWndExtra = 64;
	window_class.cbSize = sizeof(window_class);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = window_proc;
	window_class.hInstance = instance;
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.lpszClassName = "Window Class";
	RegisterClassEx(&window_class);
	
	RECT window_rect = { 0, 0, width, height };
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	HWND window = CreateWindow(window_class.lpszClassName, window_title,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		nullptr,        // We have no parent window.
		nullptr,        // We aren't using menus.
		instance, pointer_to_data);

	return window;
}

void win32_startup(winmain_parameters* winmainArguments, const char* windowTitle, engine_window* window, win32_application* win32_app)
{
	win32_app->handles.winmainArguments = *winmainArguments;
	win32_setupConsole(windowTitle);
	win32_app->applicationState.s[APPLICATION_STATE_RUNNING] = true;
	win32_app->applicationState.s[APPLICATION_STATE_RESIZING] = false;
	win32_app->applicationState.s[APPLICATION_STATE_PAUSED] = false;
	(void)QueryPerformanceFrequency((LARGE_INTEGER*)&win32_app->timer.performanceFrequency);
	(void)QueryPerformanceCounter((LARGE_INTEGER*)&win32_app->timer.firstPerformanceCounter);
	win32_app->handles.window = win32_create_window(win32_app->handles.winmainArguments.instance, win32_message_callback, (u32)window->width, (u32)window->height, windowTitle, &win32_app->applicationState);
	(void)ShowWindow(win32_app->handles.window, SW_SHOW);
}
#endif
