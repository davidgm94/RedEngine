//
// Created by david on 11/2/19.
//
#pragma once
#include "red_common.h"
#include "red_platform.h"


#include <stdio.h>
#include <time.h>
#include "linux/args.h"
int platform_Sprintf(char* buffer, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    int result = vsprintf(buffer, format, args);
    va_end(args);
    return result;
}

#include <pthread.h>
typedef enum {
   RESULT_ERROR,
   RESULT_SUCCESS
}result_t;
typedef struct {
    pthread_t threadHandle;
    result_t error;
} thread_info;

void* foo(void* parameter)
{
    int param = *((int*)parameter);
    printf("Thread %d started\n", param);
}
thread_info platform_createThread(void*(*fn)(void*), void* param)
{
    pthread_t thread;
    if (pthread_create(&thread, null, fn, param)) {
        return (thread_info) {thread, RESULT_SUCCESS};
    }
    else
    {
        return (thread_info) {null, RESULT_ERROR};
    }
}

typedef enum
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
} LOGGER_LEVEL;

void logger(LOGGER_LEVEL loggerLevel, const char* tag, const char* message)
{
    puts(message);
}
#include <math.h>

static inline i32 getSystemMiliseconds()
{
    struct timespec ts;
    int result = clock_gettime(CLOCK_MONOTONIC, &ts);
    if (result != 0)
    {
        logger(ERROR, "[CLOCK]", "Error getting the clock from system");
    }
    i32 seconds = ts.tv_sec;
    i32 nanoSeconds = ts.tv_nsec;
    i32 milliSeconds = 1000 * seconds + /*round*/((double)nanoSeconds / 1.0e6);
    return milliSeconds;
}

void platform_DebugInfo(const char* message)
{
   puts(message);
}
