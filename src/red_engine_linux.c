//
// Created by david on 11/5/19.
//
#include "red_os.h"
int main()
{
    char* message = "hello";
    char buffer[100];
    int i = 5;
    i32 start = getSystemMiliseconds();
    for (int i = 0;  i < 10; i++)
        platform_createThread(foo, &i);
    i32 end = getSystemMiliseconds();
    u64 timeElapsed = end - start;
    printf("%llu ms", timeElapsed);
}