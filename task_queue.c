
#include "task_queue.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>



int task_queue_push(struct task_queue* task_queue,
    void (*function)(enum task_action action, void* capture),
    void* capture,
    size_t capture_size)
{
    assert(task_queue->capacity > 0);
    enum task_action action = TASK_CANCELED;

    if (1)
    {
        if (capture_size > sizeof(task_queue->head->small_memory))
        {
            action = TASK_CANCELED;
            goto error;
        }

        if (task_queue->count >= task_queue->capacity)
        {
            action = TASK_FULL;
            goto error;
        }

        if (task_queue->tasks == NULL)
        {            
            task_queue->tasks = calloc(task_queue->capacity, sizeof(struct task));
            if (task_queue->tasks == NULL)
            {
                action = TASK_FULL;
                goto error;
            }         
            task_queue->head = task_queue->tasks;
            task_queue->tail = task_queue->tasks;
        }

        task_queue->head->function = function;
        memcpy(task_queue->head->small_memory, capture, capture_size);
        task_queue->head++;

        if (task_queue->head == (task_queue->tasks + task_queue->capacity))
        {
            task_queue->head = task_queue->tasks;
        }
        task_queue->count++;
    }
    else error:
    {
        function(action, capture);
        return 1;
    }
    return 0;
}

struct task* task_queue_pop(struct task_queue* ptask_queue)
{
    if (ptask_queue->count == 0)
        return NULL;

    struct task* ptask = ptask_queue->tail;
    ptask_queue->tail++;
    if (ptask_queue->tail == (ptask_queue->tasks + ptask_queue->capacity))
    {
        ptask_queue->tail = ptask_queue->tasks;
    }
    ptask_queue->count--;

    return ptask;
}


void task_queue_destroy(struct task_queue* ptask_queue)
{
    if (ptask_queue->count == 0)
        return;

    struct task* ptask = task_queue_pop(ptask_queue);
    while (ptask)
    {
        ptask->function(TASK_CANCELED, &ptask->small_memory);
        ptask = task_queue_pop(ptask_queue);
    }
    free(ptask_queue->tasks);
}

void task_queue_swap(struct task_queue* pA, struct task_queue* pB)
{
    struct task_queue t = *pA;
    *pA = *pB;
    *pB = t;
}
