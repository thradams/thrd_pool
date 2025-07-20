#include "thrd_pool.h"
#include <stdio.h>

struct thrd_pool global_thrd_pool = { 0 };


int main()
{
    if (thrd_pool_init(&global_thrd_pool, 100, 10) != 0)
        return 1;

    struct Capture { int i; } capture = { 123 };
    void func(errno_t error, void* data)
    {
       struct Capture * captured = data;
       printf("%d\n", captured->i);
    }

    errno_t e = async(func, &capture, sizeof capture);

    thrd_pool_stop(&global_thrd_pool);
    thrd_pool_join(&global_thrd_pool);
}