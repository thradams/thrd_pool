
#include "thrd_pool.h"
#include <stdlib.h>


static void thrd_pool_loop(void* data)
{
    struct thrd_pool* thrd_pool = data;

    for (;;)
    {
        mtx_lock(&thrd_pool->mutex);
        while (!thrd_pool->stopping && thrd_pool->task_queue.count == 0)
        {
            cnd_wait(&thrd_pool->condition, &thrd_pool->mutex);
        }

        if (thrd_pool->stopping)
        {
            mtx_unlock(&thrd_pool->mutex);
            break;
        }
        else
        {
            struct task* p = task_queue_pop(&thrd_pool->task_queue);
            struct task copy;
            if (p)
                copy = *p;
            mtx_unlock(&thrd_pool->mutex);

            if (p)
                copy.function(TASK_RUN, &copy.small_memory);
        }
    }

}


errno_t thrd_pool_init(struct thrd_pool* thrd_pool, size_t tasksCapacity, size_t nthreads)
{
    errno_t er = 0;

    bool mutex_created = false;
    bool condition_created = false;
    size_t num_threads_created = 0;

    if (1)
    {
        if (thrd_pool->threads != 0)
        {
            er = EFAULT;
            goto error;
        }

        if (tasksCapacity == 0)
        {
            er = ERANGE;
            goto error;
        }

        thrd_pool->threads = calloc(nthreads, sizeof thrd_pool->threads[0]);
        if (thrd_pool->threads == NULL)
        {
            er = ENOMEM;
            goto error;
        }

        thrd_pool->task_queue.capacity = tasksCapacity;
        thrd_pool->nthreads = nthreads;
        thrd_pool->stopping = true;

        int r = mtx_init(&thrd_pool->mutex, mtx_plain);
        if (r != thrd_success)
            goto error;
        mutex_created = true;

        r = cnd_init(&thrd_pool->condition);
        if (r != thrd_success)
            goto error;

        condition_created = true;
        thrd_pool->stopping = false;

        for (int i = 0; i < thrd_pool->nthreads; i++)
        {
            r = thrd_create(&thrd_pool->threads[i], &thrd_pool_loop, (void*)thrd_pool);
            if (r != thrd_success)
                goto error;
            num_threads_created++;
        }

    }
    else error:
    {
        free(thrd_pool->threads);

        if (mutex_created)
            mtx_destroy(&thrd_pool->mutex);

        if (condition_created)
            cnd_destroy(&thrd_pool->condition);

        for (size_t i = 0; i < num_threads_created; i++)
        {
            thrd_join(&thrd_pool->threads[i], NULL);
        }

        task_queue_destroy(&thrd_pool->task_queue);
    }

    return er;
}

errno_t thrd_pool_push(struct thrd_pool* thrd_pool,
    void (*function)(enum task_action action, void* capture),
    void* capture,
    size_t capture_size)
{
    errno_t result = 0;
    mtx_lock(&thrd_pool->mutex);

    if (1)
    {
        if (thrd_pool->stopping)
        {
            result = ECANCELED;
            mtx_unlock(&thrd_pool->mutex);
            goto error;
        }

        result = task_queue_push(&thrd_pool->task_queue, function, capture, capture_size);
        if (result != 0)
        {
            mtx_unlock(&thrd_pool->mutex);
            goto error;
        }

        mtx_unlock(&thrd_pool->mutex);
        cnd_signal(&thrd_pool->condition);
    }
    else error:
    {
        function(TASK_CANCELED, capture);
    }

    return result;
}


void thrd_pool_stop(struct thrd_pool* thrd_pool)
{
    if (1)
    {
        mtx_lock(&thrd_pool->mutex);
        if (thrd_pool->stopping)
        {
            mtx_unlock(&thrd_pool->mutex);
            goto error;
        }
        thrd_pool->stopping = true;
        mtx_unlock(&thrd_pool->mutex);
        cnd_broadcast(&thrd_pool->condition);
    }
    else error:
    {
        mtx_unlock(&thrd_pool->mutex);
    }
}

void thrd_pool_join(struct thrd_pool* thrd_pool)
{
    for (size_t i = 0; i < thrd_pool->nthreads; ++i)
    {
        int res = 0;
        thrd_join(thrd_pool->threads[i], &res);
    }

    mtx_destroy(&thrd_pool->mutex);
    cnd_destroy(&thrd_pool->condition);
    task_queue_destroy(&thrd_pool->task_queue);
}

errno_t async(void (*function)(enum task_action action, void* capture),
    void* capture,
    size_t capture_size)
{
    return thrd_pool_push(&global_thrd_pool, function, capture, capture_size);
}