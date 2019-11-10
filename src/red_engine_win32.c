#if _WIN64
#include "red_os.h"
#include "red_graphics.h"

#define WINDOW_TITLE "Red Engine"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 576
#define TEST 0

const u32 initialWidth = WINDOW_WIDTH;
const u32 initialHeight = WINDOW_HEIGHT;

#if TEST == 1

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	winmain_parameters winmainParameters = { .instance = hInstance, . previousInstance = hPrevInstance, .commandLineArguments = lpCmdLine, .showCommand = nCmdShow };
	engine_window initialResolution = { initialWidth, initialHeight };
	win32_startup(&winmainParameters, WINDOW_TITLE, &initialResolution, &win32);
	platform_dependencies win32_vulkan_dependencies = { .windowHandle = win32.handles.window, .anotherOSHandle = win32.handles.winmainArguments.instance };

	vulkan_renderer vk;
	QPC(startVk);
	vk_load(&vk, &win32_vulkan_dependencies);
	QPC(endVk);

	printf("%f", (float)(endVk - startVk) / win32.timer.performanceFrequency);

	while (platform_Running(&win32.applicationState))
	{
		platform_HandleEvents(win32.handles.window);
	}

	vk_destroy(&vk);
	return 0;
}

#else

typedef struct
{
	char* stringToPrint;
} work_queue_entry;

u32 volatile entryCount = 0;
u32 volatile nextEntryToDo = 0;
work_queue_entry entries[256];

static void pushString(const char* string)
{
	red_assert(entryCount < ArrayCount(entries));
	work_queue_entry* entry = &entries[entryCount];
	entry->stringToPrint = string;

	CompletePastWritesBeforeFutureWrites;
	
	++entryCount;
}

typedef struct
{
	u32 threadID;
} win32_thread_info;

unsigned long foo(void* threadParameters)
{
	win32_thread_info* threadInfo = (win32_thread_info*)threadParameters;
	char message[1000];
	for (;;)
	{
		if (nextEntryToDo < entryCount)
		{
			// TODO: not interlocked
			u32 entryIndex = InterlockedIncrement((volatile LONG*)&nextEntryToDo) - 1;

			work_queue_entry* entry = &entries[entryIndex];
			char buffer[256];
			wsprintf(buffer, "Thread %u: %s\n", threadInfo->threadID, entry->stringToPrint);
			OutputDebugStringA(buffer);
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	u32 logicalThreads = platform_getLogicalCoreCount();
	u32 threadID[logicalThreads];
	HANDLE threads[logicalThreads];
	win32_thread_info threadInfo[logicalThreads];
	for (u32 i = 0; i < logicalThreads; i++)
	{
		threadInfo[i].threadID = i;
		threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)foo, (void*)&threadInfo[i], 0, &threadID[i]);
		CloseHandle(threads[i]);
	}

	pushString("String 0");
	pushString("String 1");
	pushString("String 2");
	pushString("String 3");
	pushString("String 4");
	pushString("String 5");
	pushString("String 6");
	pushString("String 7");
}
#endif
#endif
