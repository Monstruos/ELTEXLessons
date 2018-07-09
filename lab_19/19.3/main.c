#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "errno.h"

#define THREAD_COUNT 5

void *thread_func(void *args)
{
    char *err_str;
    for (int it = 0; it < 5; it++) {
        err_str = errno(it);
        printf("Thread[%i]:%s | (addr: %x)\n", *(int *)args, err_str, err_str);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t err_th[THREAD_COUNT];
    int thread_index[THREAD_COUNT];
    for (int it = 0; it < THREAD_COUNT; it++) {
        thread_index[it] = it + 1;
        int error_code = pthread_create(&err_th[it],
                                        NULL,
                                        thread_func,
                                        &thread_index[it]);
        if (error_code != 0)
            return 1;
    }
    char *err_str;
    for (int it = 0; it < 5; it++) {
        err_str = errno(it);
        printf("Main:%s | (addr: %x)\n", err_str, err_str);
    }
    for (int it = 0; it < THREAD_COUNT; it++) {
        pthread_join(err_th[it], NULL);
    }
    return 0;
}
