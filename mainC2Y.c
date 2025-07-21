
/*
    THIS FILE IS NOT PART OF BUILD
    IT SHOWS POTENTIAL FEAUTURES IN C2Y
*/

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

    struct  Capture{
        char *user;
        char *password;
        void (*callback)(int result, char *data, void *);
        void *data;
    } capture = {0};
    
    void capture_free(struct Capture* capture) {
        free(capture->data);
        free(capture->password);
        free(capture->user);
    }


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
            memccpy(capture.data, data, 1, data_size);
        }

        errno_t e = async((void (errno_t error, void *data))
        {
            struct Capture *captured = data;            
            char *result = "result";
            captured->callback(error, result, captured->data);
            capture_free(captured);
            
        }, &capture, sizeof capture);
    }
    else error:
    {
        capture_free(&capture);
        /*always called*/         
        callback(1, "", data);
    }
}

int main()
{
    if (thrd_pool_init(&global_thrd_pool, 100, 10) != 0)
        return 1;

    do_this_async("username", "password",
                  (void(int result, char *content, void *data))
    { 
        printf("Result: %d, Content: %s\n", result, content); 
    }, NULL, 0);

    printf("waiting 5s.\n");
    struct timespec duration = {.tv_sec = 5};
    thrd_sleep(&duration, NULL);

    printf("stopping.\n");
    thrd_pool_stop(&global_thrd_pool);

    printf("join threads.\n");
    thrd_pool_join(&global_thrd_pool);

    printf("All tasks completed.\n");
}