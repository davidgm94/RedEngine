#pragma once
#include "red_common.h"

#define FORCE_INLINE 0

#if _MSC_VER
#if FORCE_INLINE
#define inline __forceinline
#else
#define inline inline
#endif

#elif __GNUC__
#if FORCE_INLINE
#define inline inline __attribute__((always_inline))
#else
#define inline inline
#endif

#endif

#define CompletePastWritesBeforeFutureWrites _WriteBarrier(); _mm_sfence()
#define CompletePastReadsBeforeFutureReads _ReadBarrier()
enum counter_type
{
	DebugCycleCount,
};

typedef struct
{
	u64 cycleCount;
	u64 hitCount;
} debug_cycle_counter;


extern struct engine_memory* debugGlobalMemory;

#define BEGIN_TIMED_BLOCK(id) u64 startCycleCount##id = PLATFORM_RDSTC()
#define END_TIMED_BLOCK(id) debugGlobalMemory->counters[debug_cycle_counter##id].cycleCount += PLATFORM_RDTSC() - startCycleCount##id;\
++debugGlobalMemory->counters[debug_cycle_counter##id].hitCount;
#define END_TIMED_BLOCK_COUNTED(id, count) debugGlobalMemory->counters[debug_cycle_counter##id].cycleCount += PLATFORM_RDTSC() - startCycleCount##id;\
debugGlobalMemory->counters[debug_cycle_counter##id].hitCount += (count);


typedef struct application_info
{
	bool s[4];
} application_state;

enum
{
	APPLICATION_STATE_RUNNING = 0x00,
	APPLICATION_STATE_RESIZING = 0x01,
	APPLICATION_STATE_PAUSED = 0x02
};

typedef struct
{
	u32 width;
	u32 height;
} engine_window;

typedef struct
{
	u32 halfTransitionCount;
	bool endedDown;
} engine_key_state;

typedef struct
{
	char keyState[UINT8_MAX + 1];
} engine_keyboard_state;

enum platform_keys
{
	MOUSE_LEFT_BUTTON = 0x01,
	MOUSE_RIGHT_BUTTON = 0x02,
	CANCEL = 0x03,
	MOUSE_MIDDLE_BUTTON = 0x04,
	MOUSE_X1_BUTTON = 0x05,
	MOUSE_X2_BUTTON = 0x06,
	UNDEFINED = 0x07,
	KEYBOARD_BACKSPACE = 0x08,
	KEYBOARD_TAB = 0x09,
	KEYBOARD_CLEAR = 0x0C,
	KEYBOARD_ENTER = 0x0D,
	KEYBOARD_SHIFT = 0x10,
	KEYBOARD_CONTROL = 0x11,
	KEYBOARD_ALT = 0x12,
	KEYBOARD_PAUSE = 0x13,
	KEYBOARD_CAPS_LOCK = 0x14,
	KEYBOARD_KANA_HANGUEL = 0x15,
	KEYBOARD_JUNJA = 0x17,
	KEYBOARD_FINAL = 0x18,
	KEYBOARD_HANJA = 0x19,
	KEYBOARD_ESCAPE = 0x1B,
	KEYBOARD_SPACE = 0x20,
	KEYBOARD_PAGE_UP = 0x21,
	KEYBOARD_PAGE_DOWN = 0x22,
	KEYBOARD_END = 0x23,
	KEYBOARD_HOME = 0x24,
	KEYBOARD_LEFT = 0x25,
	KEYBOARD_UP = 0x26,
	KEYBOARD_RIGHT = 0x27,
	KEYBOARD_DOWN = 0x28,
	KEYBOARD_SELECT = 0x29,
	KEYBOARD_PRINT = 0x2A,
	KEYBOARD_EXECUTE = 0x2B,
	KEYBOARD_PRINT_SCREEN = 0x2C,
	KEYBOARD_INSERT = 0x2D,
	KEYBOARD_DELETE = 0x2E,
	KEYBOARD_HELP = 0x2F,
	KEYBOARD_0 = 0x31,
	KEYBOARD_1 = 0x32,
	KEYBOARD_2 = 0x33,
	KEYBOARD_3 = 0x34,
	KEYBOARD_4 = 0x35,
	KEYBOARD_5 = 0x36,
	KEYBOARD_6 = 0x37,
	KEYBOARD_7 = 0x38,
	KEYBOARD_8 = 0x39,
	KEYBOARD_9 = 0x3A,
	KEYBOARD_A = 0x41,
	KEYBOARD_B = 0x42,
	KEYBOARD_C = 0x43,
	KEYBOARD_D = 0x44,
	KEYBOARD_E = 0x45,
	KEYBOARD_F = 0x46,
	KEYBOARD_G = 0x47,
	KEYBOARD_H = 0x48,
	KEYBOARD_I = 0x49,
	KEYBOARD_J = 0x4A,
	KEYBOARD_K = 0x4B,
	KEYBOARD_L = 0x4C,
	KEYBOARD_M = 0x4D,
	KEYBOARD_N = 0x4E,
	KEYBOARD_O = 0x4F,
	KEYBOARD_P = 0x50,
	KEYBOARD_Q = 0x51,
	KEYBOARD_R = 0x52,
	KEYBOARD_S = 0x53,
	KEYBOARD_T = 0x54,
	KEYBOARD_U = 0x55,
	KEYBOARD_V = 0x56,
	KEYBOARD_W = 0x57,
	KEYBOARD_X = 0x58,
	KEYBOARD_Y = 0x59,
	KEYBOARD_Z = 0x5A,
	KEYBOARD_LEFT_WINDOWS = 0x5B,
	KEYBOARD_RIGHT_WINDOWS = 0x5C,
	KEYBOARD_APPLICATION = 0x5D,
	KEYBOARD_SLEEP = 0x5F,
	KEYBOARD_NOTEPAD_0 = 0x60,
	KEYBOARD_NOTEPAD_1 = 0x61,
	KEYBOARD_NOTEPAD_2 = 0x62,
	KEYBOARD_NOTEPAD_3 = 0x63,
	KEYBOARD_NOTEPAD_4 = 0x64,
	KEYBOARD_NOTEPAD_5 = 0x65,
	KEYBOARD_NOTEPAD_6 = 0x66,
	KEYBOARD_NOTEPAD_7 = 0x67,
	KEYBOARD_NOTEPAD_8 = 0x68,
	KEYBOARD_NOTEPAD_9 = 0x69,
	KEYBOARD_MULTIPLY = 0x6A,
	KEYBOARD_ADD = 0x6B,
	KEYBOARD_SEPARATOR = 0x6C,
	KEYBOARD_SUBSTRACT = 0x6D,
	KEYBOARD_DECIMAL = 0x6E,
	KEYBOARD_DIVIDE = 0x6F,
	KEYBOARD_F1 = 0x70,
	KEYBOARD_F2 = 0x71,
	KEYBOARD_F3 = 0x72,
	KEYBOARD_F4 = 0x73,
	KEYBOARD_F5 = 0x74,
	KEYBOARD_F6 = 0x75,
	KEYBOARD_F7 = 0x76,
	KEYBOARD_F8 = 0x77,
	KEYBOARD_F9 = 0x78,
	KEYBOARD_F10 = 0x79,
	KEYBOARD_F11 = 0x7A,
	KEYBOARD_F12 = 0x7B,
	KEYBOARD_F13 = 0x7C,
	KEYBOARD_F14 = 0x7D,
	KEYBOARD_F15 = 0x7E,
	KEYBOARD_F16 = 0x7F,
	KEYBOARD_F17 = 0x80,
	KEYBOARD_F18 = 0x81,
	KEYBOARD_F19 = 0x82,
	KEYBOARD_F20 = 0x83,
	KEYBOARD_F21 = 0x84,
	KEYBOARD_F22 = 0x85,
	KEYBOARD_F23 = 0x86,
	KEYBOARD_F24 = 0x87,
	KEYBOARD_NUMLOCK = 0x90,
	KEYBOARD_SCROLL = 0x91,
	KEYBOARD_LEFT_SHIFT = 0xA0,
	KEYBOARD_RIGHT_SHIFT = 0xA1,
	KEYBOARD_LEFT_CONTROL = 0xA2,
	KEYBOARD_RIGHT_CONTROL = 0xA3,
	KEYBOARD_LEFT_MENU = 0xA4,
	KEYBOARD_RIGHT_MENU = 0xA5,
	KEYBOARD_BROWSER_BACK = 0xA6,
	KEYBOARD_BROWSER_FORWARD = 0xA7,
	KEYBOARD_BROWSER_REFRESH = 0xA8,
	KEYBOARD_BROWSER_STOP = 0xA9,
	KEYBOARD_BROWSER_SEARCH = 0xAA,
	KEYBOARD_BROWSER_FAVORITES = 0xAB,
	KEYBOARD_BROWSER_HOME = 0xAC,
	KEYBOARD_VOLUME_MUTE = 0xAD,
	KEYBOARD_VOLUME_DOWN = 0xAE,
	KEYBOARD_VOLUME_UP = 0xAF,
	KEYBOARD_MEDIA_NEXT_TRACK = 0xB0,
	KEYBOARD_MEDIA_PREV_TRACK = 0xB1,
	KEYBOARD_MEDIA_STOP = 0xB2,
	KEYBOARD_MEDIA_PLAY_PAUSE = 0xB3,
	KEYBOARD_LAUNCH_MAIL = 0xB4,
	KEYBOARD_LAUNCH_MEDIA_SELECT = 0xB5,
	KEYBOARD_LAUNCH_APP1 = 0xB6,
	KEYBOARD_LAUNCH_APP2 = 0xB7,
	// (...)
};

typedef struct
{
    void* windowHandle;
    void* anotherOSHandle;
} platform_dependencies;

#define PLATFORM_DEBUG_BREAK(name) void name(void)
typedef PLATFORM_DEBUG_BREAK(platform_debug_break);

#define PLATFORM_DEBUG_INFO(name) void name(const char* message)
typedef PLATFORM_DEBUG_INFO(platform_debug_info);

#if MEASURE_PERFORMANCE
#define PLATFORM_DEBUG_COUNTER(name) void name(const char* string, int counter)
typedef PLATFORM_DEBUG_COUNTER(platform_debug_counter);
PLATFORM_DEBUG_COUNTER(platform_DebugCounter);
#define DEBUG_COUNTER(string, counter) (platformDebugCounter(#string, counter))
#else
#define DEBUG_COUNTER(string, counter)
#endif

#if NDEBUG
#define red_assert(condition)
#else
#define red_assert(condition) if (!(condition)) { platform_DebugInfo("[ASSERT FAILED] The expression " #condition " is false\n"); platform_DebugBreak(); }
#define msg_red_assert(condition, message) if (!(condition)) { platform_DebugInfo("[ASSERT FAILED] The expression " #condition " is false\n[LOCATION] " #message "\n\n"); platform_DebugBreak(); }
#endif

#define assert(condition) red_assert(condition)
#define msg_assert(condition, message) msg_red_assert(condition, message)
#define invalid_code_path assert(!"Invalid code path")
#define invalid_default_case default: { invalid_code_path; break; }

#include <stdio.h>

#define PLATFORM_MEMCPY(name) void* name(void* destination, void* source, size_t size)
typedef PLATFORM_MEMCPY(platform_memcpy);

#define PLATFORM_SPRINTF(name) int name(char* const buffer, char const* const format, ...)
typedef PLATFORM_SPRINTF(platform_sprintf);

#define PLATFORM_FPRINTF(name) int name(FILE* const stream, char const* const format, ...)
typedef PLATFORM_FPRINTF(platform_fprintf);

#define PLATFORM_PRINTF(name) int name(char const* const format, ...)
typedef PLATFORM_PRINTF(platform_printf);

#define PLATFORM_STRCPY(name) char* name(char* destination, char const* source)
typedef PLATFORM_STRCPY(platform_strcpy);

#define PLATFORM_STRLEN(name) size_t name(char const* string)
typedef PLATFORM_STRLEN(platform_strlen);

#define PLATFORM_STRCMP(name) int name(char const* str1, char const* str2)
typedef PLATFORM_STRCMP(platform_strcmp);

#define PLATFORM_PERFORMANCE_COUNTER(name) i64 name(void)
typedef PLATFORM_PERFORMANCE_COUNTER(platform_performance_counter);

#define PLATFORM_RDTSC(name) u64 name(void)
typedef PLATFORM_RDTSC(platform_rdtsc);

#define PLATFORM_GET_TIME(name) float name(void)
typedef PLATFORM_GET_TIME(platform_get_time);

#define PLATFORM_GET_TIME_ELAPSED(name) float name(void)
typedef PLATFORM_GET_TIME_ELAPSED(platform_get_time_elapsed);

#define PLATFORM_IS_RUNNING(name) bool name(void* platform)
typedef PLATFORM_IS_RUNNING(platform_is_running);

// TODO: maybe this is wrong and offers poor portability? check in the future
#define PLATFORM_HANDLE_EVENTS(name) void name(void* platform)
typedef PLATFORM_HANDLE_EVENTS(platform_handle_events);

#define PLATFORM_GET_LOGICAL_CORE_COUNT(name) u32 name(void)
typedef PLATFORM_GET_LOGICAL_CORE_COUNT(platform_get_logical_core_count);

typedef struct
{
	void* platform;
	bool noErrors;
} platform_file_handle;

typedef struct
{
	void* platform;
	bool noErrors;
} platform_file_group;

#define PLATFORM_OPEN_FILE(name) platform_file_handle name(const char* filename)
typedef PLATFORM_OPEN_FILE(platform_open_file);

#define PLATFORM_READ_DATA_FROM_FILE(name) void name(platform_file_handle* fileHandle, void* destination)
typedef PLATFORM_READ_DATA_FROM_FILE(platform_read_data_from_file);

#define PLATFORM_FILE_ERROR(name) void name(platform_file_handle* file, char* message)
typedef PLATFORM_FILE_ERROR(platform_file_error);

#define PlatformNoFileErrors(handle) ((handle->noErrors))

typedef struct platform_job_queue platform_job_queue;
#define PLATFORM_JOB_SYSTEM_CALLBACK(name) void name(platform_job_queue* queue, void* data)
typedef PLATFORM_JOB_SYSTEM_CALLBACK(platform_job_system_callback);

#define PLATFORM_ALLOCATE_MEMORY(name) void* name(size_t size)
typedef PLATFORM_ALLOCATE_MEMORY(platform_allocate_memory);

#define PLATFORM_DEALLOCATE_MEMORY(name) void name(void* memory)
typedef PLATFORM_DEALLOCATE_MEMORY(platform_deallocate_memory);

typedef struct
{
	// TODO: initialize to 0 at startup
	size_t permanentMemorySize;
	void* permanentMemory;
	size_t temporalMemorySize;
	void*  temporalMemory;
#if MEASURE_PERFORMANCE
	// debug_cycle_counter counters[DebugCycleCount];
#endif
} engine_memory;

typedef struct engine_input engine_input;

typedef void* engine_api;
typedef struct
{
	engine_window window;
	engine_api api;
} engine_renderer;

#define APPLICATION_UPDATE_AND_RENDER(name) void name(engine_memory* memory, engine_input* input, engine_renderer* renderer)
typedef APPLICATION_UPDATE_AND_RENDER(application_update_and_render);
