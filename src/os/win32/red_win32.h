#pragma once
#if _WIN64
#define _CRT_SECURE_NO_WARNINGS 1
#include "../../red_common.h"
#include "../red_platform.h"
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
} win32_application;

win32_application win32;
void win32_startup(winmain_parameters* winmainArguments, const char* windowTitle, engine_window* window, win32_application* win32);
inline PLATFORM_IS_RUNNING(platform_Running)
{
	win32_application_state* appState = platform;
	return appState->s[0];
}

inline PLATFORM_HANDLE_EVENTS(platform_HandleEvents)
{
	HWND window = (HWND)platform;
	MSG msg;																\
	while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))						\
	{																		\
		TranslateMessage(&msg);												\
		DispatchMessage(&msg);												\
	}
}
//// TODO: erase in the future
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//inline PLATFORM_MEMCPY(platform_Memcpy)
//{
//	return memcpy(destination, source, size);
//}
//
//inline PLATFORM_SPRINTF(platform_Sprintf)
//{
//	int _Result;
//	va_list _ArgList;
//	__crt_va_start(_ArgList, format);
//	_Result = _vsnprintf_l(buffer, (size_t)-1, format, NULL, _ArgList);
//	__crt_va_end(_ArgList);
//	return _Result;
//}
//
//inline PLATFORM_FPRINTF(platform_Fprintf)
//{
//	int _Result;
//	va_list _ArgList;
//	__crt_va_start(_ArgList, format);
//	_Result = __stdio_common_vfprintf(_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS, stream, format, NULL, _ArgList);
//	__crt_va_end(_ArgList);
//	return _Result;
//}
//
//inline PLATFORM_PRINTF(platform_Printf)
//{
//	int _Result;
//	va_list _ArgList;
//	__crt_va_start(_ArgList, format);
//	_Result = __stdio_common_vfprintf(_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS, stdout, format, NULL, _ArgList);
//	__crt_va_end(_ArgList);
//	return _Result;
//}
//
//
//inline PLATFORM_STRCPY(platform_Strcpy)
//{
//	return strcpy(destination, source);
//}
//
//inline PLATFORM_STRLEN(platform_Strlen)
//{
//	return strlen(string);
//}
//
//inline PLATFORM_STRCMP(platform_Strcmp)
//{
//	return strcmp(str1, str2);
//}
//
//typedef struct
//{
//	bool s[4];
//} win32_application_state;
//
//typedef struct
//{
//	HINSTANCE instance;
//	HINSTANCE previousInstance;
//	PSTR commandLineArguments;
//	int showCommand;
//} winmain_parameters;
//
//typedef struct
//{
//	winmain_parameters winmainArguments;
//	HWND window;
//} win32_native_handles;
//
//typedef struct
//{
//	i64 performanceFrequency;
//	i64 firstPerformanceCounter;
//} win32_timer;
//
//typedef struct
//{
//	win32_native_handles handles;
//	win32_application_state applicationState;
//	win32_timer timer;
//} win32_application;
//
//win32_application win32;
//
//#define QPC(name) i64 name; (void)QueryPerformanceCounter((LARGE_INTEGER*)&name)
//
//void win32_startup(winmain_parameters* winmainArguments, const char* windowTitle, engine_window* window, win32_application* win32);
//
//
//// Window message handling
//inline PLATFORM_HANDLE_EVENTS(platform_HandleEvents)
//{
//	HWND window = (HWND)platform;
//	MSG msg;																\
//	while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))						\
//	{																		\
//		TranslateMessage(&msg);												\
//		DispatchMessage(&msg);												\
//	}
//}
//
//inline PLATFORM_DEBUG_BREAK(platform_DebugBreak)
//{
//	DebugBreak();
//}
//
//inline PLATFORM_DEBUG_INFO(platform_DebugInfo)
//{
//	OutputDebugString(message);
//}
//
//inline PLATFORM_DEBUG_COUNTER(platform_DebugCounter)
//{
//	char message[256];
//	(void)platform_Sprintf(message, "%s: %i\n", string, counter);
//	OutputDebugString(message);
//}
//
//inline PLATFORM_PERFORMANCE_COUNTER(platform_PerformanceCounter)
//{
//	QPC(performanceCounter);
//	return performanceCounter;
//}
//
//inline PLATFORM_RDTSC(platform_RDTSC)
//{
//	u64 rdtsc_ = __rdtsc();
//	return rdtsc_;
//}
//
//inline PLATFORM_GET_TIME(platform_GetTime)
//{
//	QPC(performanceCounter);
//	float time = (float)performanceCounter / (float)win32.timer.performanceFrequency;
//	return time;
//}
//
//inline PLATFORM_GET_TIME_ELAPSED(platform_GetTimeElapsed)
//{
//	QPC(performanceCounter);
//	i64 difference = performanceCounter - win32.timer.firstPerformanceCounter;
//	float time = (float)difference / win32.timer.performanceFrequency;
//
//	return time;
//}
//
//inline PLATFORM_ALLOCATE_MEMORY(platform_AllocateMemory)
//{
//	return malloc(size);
//}
//inline PLATFORM_DEALLOCATE_MEMORY(platform_DeallocateMemory)
//{
//	free(memory);
//}
//
//inline PLATFORM_OPEN_FILE(platform_OpenFile)
//{
//	platform_file_handle fileHandle;
//	FILE* file = fopen(filename, "rb");
//	bool noErrors;
//	noErrors = file;
//	fileHandle.platform = file;
//	fileHandle.noErrors = noErrors;
//
//	return fileHandle;
//}
//
//inline PLATFORM_READ_DATA_FROM_FILE(platform_ReadDataFromFile)
//{
//	fseek(fileHandle->platform, 0, SEEK_END);
//	size_t length = (size_t)ftell(fileHandle->platform);
//	char* fileText = (char*)platform_AllocateMemory(length + 1);
//	if (fileText == nullptr)
//	{
//		fileHandle->noErrors = false;
//		destination = nullptr;
//		return;
//	}
//
//	fseek(fileHandle->platform, 0, SEEK_SET);
//	fread(fileText, 1, length, fileHandle->platform);
//	fclose(fileHandle->platform);
//	fileText[length] = 0;
//	destination = fileText;
//}
//
//typedef struct
//{
//	char* c_str;
//	u64 size;
//} raw_str;
//
//inline raw_str platform_readFile(const char* fileFullPath)
//{
//	FILE* file = fopen(fileFullPath, "rb"); // read binary flag
//	assert(file);
//
//	fseek(file, 0, SEEK_END);
//	long length = ftell(file); // length (bytes) of the shader bytecode
//	assert(length >= 0);
//	fseek(file, 0, SEEK_SET);
//
//	char* fileContent = null;
//	fileContent = (char*)malloc(length + 1);
//	size_t rc = fread(&fileContent[0], 1, length, file);
//	assert(rc == (size_t)(length));
//	fclose(file);
//#if _DEBUG
//	printf("FILE READ:\n%s", fileContent);
//#endif
//
//	return (raw_str) { fileContent, length };
//}
//
//inline PLATFORM_FILE_ERROR(platform_FileError);
//inline PLATFORM_JOB_SYSTEM_CALLBACK(platform_JobSystemCallback);
//
//inline u32 safeTruncateU64(u64 value)
//{
//	assert(value <= UINT32_MAX)
//	u32 result = (u32)value;
//	return result;
//}
//
//inline PLATFORM_GET_LOGICAL_CORE_COUNT(platform_getLogicalCoreCount)
//{
//	SYSTEM_INFO systemInfo;
//	GetSystemInfo(&systemInfo);
//	u32 logicalThreads = systemInfo.dwNumberOfProcessors;
//	return logicalThreads;
//}
//#define qprint(something) platform_DebugInfo(something)
//
//#endif
