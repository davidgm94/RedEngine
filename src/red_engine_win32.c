#include "red_os.h"
#include "red_graphics.h"

#define WINDOW_TITLE "Red Engine"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 576

const u32 initialWidth = WINDOW_WIDTH;
const u32 initialHeight = WINDOW_HEIGHT;

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