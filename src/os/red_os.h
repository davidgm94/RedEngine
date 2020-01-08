#pragma once

#if _WIN64
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#if _WIN64
#if _DEBUG
#define NDEBUG 0
#else
#define NDEBUG 1
#endif
#else
#endif

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

#define ArrayCount(array) (sizeof(array) / sizeof(array[0]))

// TYPES
#ifndef __cplusplus
#define bool	_Bool
#define false	0
#define true	1
#define nullptr NULL
#endif
#define null nullptr

    typedef signed char        i8;
    typedef short              i16;
    typedef int                i32;
    typedef long long          i64;
    typedef unsigned char      u8;
    typedef unsigned short     u16;
    typedef unsigned int       u32;
    typedef unsigned long long u64;

	//typedef u64 size_t;
    typedef i64 intptr;
    typedef u64 uintptr;
    typedef unsigned int uint;

#define BYTE_SIZE 1Ui64
#define KILOBYTE (1024 * BYTE_SIZE)
#define MEGABYTE (1024 * KILOBYTE)
#define GIGABYTE (1024 * MEGABYTE)
// ASSERT
#include <assert.h>
// ERROR				??
// MATH					??
#include <math.h>
// SIGNAL
#include <signal.h>
// ALIGNMENT
// VARIABLE ARGS
#include <stdarg.h>
// ATOMIC TYPES
// BOOLEAN
#include <stdbool.h>
// STDDEF
#include <stddef.h>
	// OFFSETS: #define offsetof(s,m) ((size_t)&(((s*)0)->m))
// STDIO
#include <stdio.h>
// STRING
#include <string.h>
// THREAD SYSTEM
// TIME
// MEMORY
#include <stdlib.h>
// WINDOW
    typedef void* os_window;
    struct os_window_list_entry
    {
        os_window window;
        os_window_list_entry* next;
    }; //os_window_list_entry;
// WINDOW LOOP
    bool os_windowShouldClose(os_window* window);
    void os_windowHandleEvents(os_window* window);
// SHELL
// AUDIO
// NETWORKING

// LOGGER
#define SHOW_DEBUG 1
#define SHOW_WARNING 1
#define SHOW_ERROR 1
#define SHOW_CRITICAL 1
    typedef enum
    {
        DEBUG,
        WARNING,
        ERROR,
        CRITICAL
    } RED_LOG_SEVERITY;

// CONTAINERS
#define fast_vector_malloc(type, count) (type*)malloc(sizeof(type) * count)
#define fast_vector_free(pointer) free(pointer)
#define FAST_VECTOR(type) typedef struct { type* data; size_t size; } fast_vector_##type;\
    static inline void allocate_fast_vector_##type(fast_vector_##type* vector, size_t count)\
    {\
        vector->data = fast_vector_malloc(type, count);\
        vector->size = count;\
    }\
    \
    static inline void deallocate_fast_vector_##type(fast_vector_##type* vector, size_t count)\
    {\
        fast_vector_free(vector->data);\
        vector->data = null;\
        vector->size = 0;\
    }

    FAST_VECTOR(u32);
typedef void* os_manager;
static os_manager currentPlatform = null;

typedef struct
{
    void* windowHandle;
    void* anotherOSHandle;
} os_handles;

typedef struct
{
	u32 width;
	u32 height;
} os_window_dimensions;

void os_startup(void);
os_handles os_getPlatformHandles(void);
os_window_dimensions os_getWindowDimensions(void);
#ifdef __cplusplus
}
#endif
