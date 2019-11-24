//
// Created by david on 11/5/19.
//
#include "red_os.h"
#include "red_graphics.h"
#include <GLFW/glfw3.h>
#include <external/volk/volk.h>

#define TEST 0
#if TEST
int main()
{
    char* message = "hello";
    char buffer[100];
    int i = 5;
    i64 start = getSystemNanoseconds();
    for (int i = 0;  i < 10; i++)
        platform_createThread(foo, &i);
    int* array = platform_alloc(5);
    timer s;
    (void)timer_update(&s, CLOCK_MONOTONIC);
    i64 msStart = timer_popMs(&s);
    i64 nsStart = timer_popNs(&s);
    printf("%llu ms\n", msStart);
    printf("%llu ns\n", nsStart);
    (void)timer_update(&s, CLOCK_MONOTONIC);
    i64 msEnd = timer_popMs(&s);
    i64 nsEnd = timer_popNs(&s);
    printf("%llu ms\n", msEnd);
    printf("%llu ns\n", nsEnd);
    printf("Diff: %llu ms, %llu ns\n", msEnd - msStart, nsEnd - nsStart);

    for (int i =0; i < 5; i++)
    {
        array[i] = 5;
        printf("%d\n", array[i]);
    }
    i64 end = getSystemNanoseconds();
    u64 timeElapsed = end - start;
    printf("%llu ns", timeElapsed);
}
#else
int main()
{
    VkAllocationCallbacks* allocator = nullptr;

    VkQueueFlags availableQueues[] = { VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT };
    VkQueueFlags queuesToCreate = 0;
    u32 availableQueueCount = ArrayCount(availableQueues);
    for (u32 i = 0; i < availableQueueCount; i++)
    {
        queuesToCreate |= availableQueues[i];
    }

    // Using volk code to load Vulkan function pointers
    VKCHECK(volkInitialize());
    VkInstance instance = vk_createInstance(allocator);
    volkLoadInstance(instance);

    return 0;
}
#endif