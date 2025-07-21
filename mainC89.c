struct __pthread_internal_list {
    struct __pthread_internal_list * __prev;
    struct __pthread_internal_list * __next;
};

struct __pthread_mutex_s {
    int __lock;
    unsigned int __count;
    int __owner;
    unsigned int __nusers;
    int __kind;
    short __spins;
    short __elision;
    struct __pthread_internal_list  __list;
};

struct task {
    void (* function)(int error, void * capture);
    char small_memory[400];
};

struct _struct_tag_3 {
    unsigned int __low;
    unsigned int __high;
};

union _struct_tag_2 {
    unsigned int long long __value64;
    struct _struct_tag_3  __value32;
};

struct __pthread_cond_s {
    union _struct_tag_2  __wseq;
    union _struct_tag_2  __g1_start;
    unsigned int __g_refs[2];
    unsigned int __g_size[2];
    unsigned int __g1_orig_size;
    unsigned int __wrefs;
    unsigned int __g_signals[2];
};

struct task_queue {
    struct task * tasks;
    unsigned int long capacity;
    unsigned int long count;
    struct task * head;
    struct task * tail;
};

union _struct_tag_7 {
    struct __pthread_mutex_s  __data;
    char __size[40];
    int long __align;
};

union _struct_tag_8 {
    struct __pthread_cond_s  __data;
    char __size[48];
    int long long __align;
};

struct thrd_pool {
    struct task_queue  task_queue;
    union _struct_tag_7  mutex;
    unsigned int long * threads;
    unsigned int long nthreads;
    unsigned char   stopping;
    union _struct_tag_8  condition;
};

struct Capture {
    char * user;
    char * password;
    void (* callback)(int result, char * data, void *);
    void * data;
};

struct timespec {
    int long tv_sec;
    int long tv_nsec;
};


static void _cake_zmem(void *dest, register unsigned int len)
{
  register unsigned char *ptr = (unsigned char*)dest;
  while (len-- > 0) *ptr++ = 0;
}

struct thrd_pool  global_thrd_pool = {0};
extern char *strdup(char * __s);
extern void *malloc(unsigned int long __size);
extern void *memccpy(void * __dest, void * __src, int __c, unsigned int long __n);
extern void free(void * __ptr);
int async(void (* function)(int error, void * capture), void * capture, unsigned int long capture_size);

static void _ck_l_func_0(int error, void * data)
{
    char * result;
    struct Capture * captured;

    result = "result";
    captured = data;
    captured->callback(error, result, captured->data);
    free(captured->data);
    free(captured->user);
    free(captured->password);
}

int do_this_async(char * user, char * password, void (* callback)(int result, char * content, void * data), void * data, unsigned int long data_size)
{
    struct Capture  capture;

    _cake_zmem(&capture, 32);
    if (1)
    {
        int e;

        capture.callback = callback;
        capture.user = strdup(user);
        if (capture.user == 0ULL)
        {
            goto error;
        }
        capture.password = strdup(password);
        if (capture.password == 0ULL)
        {
            goto error;
        }
        if (data_size > 0)
        {
            capture.data = malloc(data_size);
            if (capture.data == 0ULL)
            {
                goto error;
            }
        }
        memccpy(capture.data, data, 1, data_size);
        e = async(_ck_l_func_0, &capture, 32ULL);
    }
    else
    {
        error:
        {
            free(capture.data);
            free(capture.password);
            free(capture.user);
        }
    }
}

int thrd_pool_init(struct thrd_pool * thrd_pool, unsigned int long tasksCapacity, unsigned int long nthreads);
extern struct thrd_pool  global_thrd_pool;
extern int printf(char * __format, ...);
int thrd_sleep(struct timespec * duration, struct timespec * remaining);
void thrd_pool_stop(struct thrd_pool * thrd_pool);
void thrd_pool_join(struct thrd_pool * thrd_pool);

static void _ck_l_func_1(int result, char * content, void * data)
{
    printf("Result: %d, Content: %s\n", result, content);
}

int main()
{
    struct timespec  duration;

    if (thrd_pool_init(&global_thrd_pool, 100, 10) != 0)
    {
        return 1;
    }
    do_this_async("username", "password", _ck_l_func_1, 0ULL, 0);
    printf("waiting 5s.\n");
    duration.tv_sec = 5;
    duration.tv_nsec = 0;
    thrd_sleep(&duration, 0ULL);
    printf("stopping.\n");
    thrd_pool_stop(&global_thrd_pool);
    printf("join threads.\n");
    thrd_pool_join(&global_thrd_pool);
    printf("All tasks completed.\n");
}

