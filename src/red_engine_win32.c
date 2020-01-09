#if _WIN64
#include "os/red_os.h"
#include "graphics/red_graphics.h"

#define WINDOW_TITLE "Red Engine"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 576
#define TEST 0

const u32 initialWidth = WINDOW_WIDTH;
const u32 initialHeight = WINDOW_HEIGHT;

#if TEST == 0

#if _WIN64
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
#if _WIN64
	winmain_parameters winmainParameters = { .instance = hInstance, . previousInstance = hPrevInstance, .commandLineArguments = lpCmdLine, .showCommand = nCmdShow };
	win32.handles.winmainArguments = winmainParameters;
#endif
	os_window_dimensions initialResolution = { initialWidth, initialHeight };
	//os_startup();
	// TODO: this should be recalled window startup?? refactor or rename
	currentWindow = os_startup(WINDOW_TITLE, &initialResolution);
	os_window_handles windowHandles = os_getWindowHandles(currentWindow);

	// graphics_load();
	vulkan_renderer vk;
	vk_load(&vk, &windowHandles, &initialResolution);

	// TODO: multiWindow code?
	while (!os_windowShouldClose(currentWindow))
	{
		// graphics_render(); TODO: upgrade info
		vk_render(&vk);
		os_windowHandleEvents(currentWindow);
	}

	// destroy_graphics();
	vk_destroy(&vk);
	// destroy_os();
	return 0;
}

#else
#if _WIN64
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
#if _WIN64
	winmain_parameters winmainParameters = { .instance = hInstance, . previousInstance = hPrevInstance, .commandLineArguments = lpCmdLine, .showCommand = nCmdShow };
	platform_dependencies vulkan_dependencies = { .windowHandle = win32.handles.window, .anotherOSHandle = win32.handles.winmainArguments.instance };
#endif
	engine_window initialResolution = { initialWidth, initialHeight };
	//os_startup();
	win32_startup(&winmainParameters, WINDOW_TITLE, &initialResolution, &win32);

	// graphics_load();
	vulkan_renderer vk;
	// cross_platform_QPC
	QPC(startVk);
	vk_load(&vk, (os_window_handler*)&vulkan_dependencies, (os_window_dimension*)&initialResolution);
	// cross_platform_QPC
	QPC(endVk);

	// cross_platform_QPC
	os_printf("Vulkan initialized in %f ms.\n", (float)(endVk - startVk) / win32.timer.performanceFrequency);

	// while (applicationRunning)
	while (platform_Running(&win32.applicationState))
	{
		//QPC(startFrame);
		// graphics_render(); TODO: upgrade info
		vk_render(&vk);
		// os_handleEvents();
		platform_HandleEvents(win32.handles.window);
		//QPC(endFrame);
		//os_printf("Frame time: %.02f ms.", (float)(endFrame - startFrame) / win32.timer.performanceFrequency);
	}

	// destroy_graphics();
	vk_destroy(&vk);
	// destroy_os();
	return 0;
}
//
//typedef struct
//{
//	char* stringToPrint;
//} work_queue_entry;
//
//u32 volatile entryCount = 0;
//u32 volatile nextEntryToDo = 0;
//work_queue_entry entries[256];
//
//static void pushString(const char* string)
//{
//	red_assert(entryCount < ArrayCount(entries));
//	work_queue_entry* entry = &entries[entryCount];
//	entry->stringToPrint = string;
//
//	CompletePastWritesBeforeFutureWrites;
//	
//	++entryCount;
//}
//
//typedef struct
//{
//	u32 threadID;
//} win32_thread_info;
//
//unsigned long foo(void* threadParameters)
//{
//	win32_thread_info* threadInfo = (win32_thread_info*)threadParameters;
//	char message[1000];
//	for (;;)
//	{
//		if (nextEntryToDo < entryCount)
//		{
//			// TODO: not interlocked
//			u32 entryIndex = InterlockedIncrement((volatile LONG*)&nextEntryToDo) - 1;
//
//			work_queue_entry* entry = &entries[entryIndex];
//			char buffer[256];
//			wsprintf(buffer, "Thread %u: %s\n", threadInfo->threadID, entry->stringToPrint);
//			OutputDebugStringA(buffer);
//		}
//	}
//}
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//	PSTR lpCmdLine, INT nCmdShow)
//{
//	u32 logicalThreads = platform_getLogicalCoreCount();
//	u32 threadID[logicalThreads];
//	HANDLE threads[logicalThreads];
//	win32_thread_info threadInfo[logicalThreads];
//	for (u32 i = 0; i < logicalThreads; i++)
//	{
//		threadInfo[i].threadID = i;
//		threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)foo, (void*)&threadInfo[i], 0, &threadID[i]);
//		CloseHandle(threads[i]);
//	}
//
//	pushString("String 0");
//	pushString("String 1");
//	pushString("String 2");
//	pushString("String 3");
//	pushString("String 4");
//	pushString("String 5");
//	pushString("String 6");
//	pushString("String 7");
//}
//#endif
#endif
#endif
