#include "thrd_pool.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct thrd_pool global_thrd_pool = {0};

errno_t do_this_async(char *user,
                      char *password,
                      void (*callback)(int result, char *content, void *data),
                      void *data,
                      size_t data_size)
{

    struct Capture
    {
        char *user;
        char *password;
        void (*callback)(int result, char *data, void *);
        void *data;
    };
    struct Capture capture = {0};

    if (1)
    {
        capture.callback = callback;
        capture.user = strdup(user);
        if (capture.user == NULL)
            goto error;

        capture.password = strdup(password);

        if (capture.password == NULL)
            goto error;

        if (data_size > 0)
        {
            capture.data = malloc(data_size);
            if (capture.data == NULL)
                goto error;
        }

        memccpy(capture.data, data, 1, data_size);
        void do_this_execute(errno_t error, void *data)
        {
            char *result = "result";

            struct Capture *captured = data;
            captured->callback(error, result, captured->data);

            free(captured->data);
            free(captured->user);
            free(captured->password);
        }

        errno_t e = async(do_this_execute, &capture, sizeof capture);
    }
    else
    error:
    {
        free(capture.data);
        free(capture.password);
        free(capture.user);
    }
}

int main()
{
    if (thrd_pool_init(&global_thrd_pool, 100, 10) != 0)
        return 1;

    void then_do_that(int result, char *content, void *data)
    {
        printf("Result: %d, Content: %s\n", result, content);
    }
    do_this_async("username", "password", then_do_that, NULL, 0);

    printf("waiting 5s.\n");
    struct timespec duration = {.tv_sec = 5};
    thrd_sleep(&duration, NULL);

    printf("stopping.\n");
    thrd_pool_stop(&global_thrd_pool);

    printf("join threads.\n");
    thrd_pool_join(&global_thrd_pool);

    printf("All tasks completed.\n");
}