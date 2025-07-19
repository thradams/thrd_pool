#include "thrd_pool.h"
#include <stdio.h>

struct thrd_pool global_thrd_pool = { 0 };


struct Capture { int i; } capture = { 1 };
void func(enum task_action action, void* data)
{
    typeof(capture)* captured = data;
    printf("%d\n", captured->i);
}

int main()
{
    if (thrd_pool_init(&global_thrd_pool, 100, 10) != 0)
        return 1;


    if (async(func, &capture, sizeof capture) != 0)
    {
    }

    thrd_pool_stop(&global_thrd_pool);
    thrd_pool_join(&global_thrd_pool);
}