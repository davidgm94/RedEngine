//
// Created by david on 11/5/19.
//
#include "red_os.h"
int main()
{
    char* message = "hello";
    char buffer[100];
    int i = 5;
    i64 start = getSystemNanoseconds();
    for (int i = 0;  i < 10; i++)
        platform_createThread(foo, &i);
    int* array = platform_alloc(5);
    for (int i =0; i < 5; i++)
    {
        array[i] = 5;
        printf("%d\n", array[i]);
    }
    i64 end = getSystemNanoseconds();
    u64 timeElapsed = end - start;
    printf("%llu ns", timeElapsed);
}