#pragma once
#include <stdbool.h>
#include "tinycthread.h"
#include "task_queue.h"



struct thrd_pool
{
  struct task_queue task_queue;
  mtx_t        mutex;
  thrd_t       *threads;
  size_t       nthreads;
  bool         stopping;
  cnd_t        condition;  
};

extern struct thrd_pool global_thrd_pool;

[[nodiscard]]
errno_t thrd_pool_init(struct thrd_pool* thrd_pool, size_t tasksCapacity, size_t nthreads);

[[nodiscard]]
errno_t thrd_pool_push(struct thrd_pool * task_queue,
                    void (*function)(errno_t error, void* capture),
                    void * capture,
                    size_t capture_size);


void thrd_pool_stop(struct thrd_pool* thrd_pool);
void thrd_pool_join(struct thrd_pool* thrd_pool);

[[nodiscard]]
errno_t async(void (*function)(errno_t error, void* capture),
    void* capture,
    size_t capture_size);
