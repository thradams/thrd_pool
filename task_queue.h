#pragma once
#include <stdint.h>
#include <stddef.h>

typedef int errno_t;

#ifdef _MSC_VER 
typedef double max_align_t;
#endif

struct task
{
  void (*function)(errno_t error, void* capture);  
  
  _Alignas(max_align_t) 
  char small_memory[sizeof(int)*100];
};


struct task_queue
{
  struct task* tasks;
  size_t capacity;
  size_t count;
  struct task* head;
  struct task* tail;
};

[[nodiscard]]
errno_t task_queue_push(struct task_queue * task_queue,
                        void (*function)(errno_t error, void* capture),
                        void * capture,
                        size_t capture_size);
[[nodiscard]]
struct task* task_queue_pop(struct task_queue *task_queue);

void task_queue_destroy(struct task_queue *ptask_task_queuequeue);
void task_queue_swap(struct task_queue *task_queue_a, struct task_queue *task_queue_b);
