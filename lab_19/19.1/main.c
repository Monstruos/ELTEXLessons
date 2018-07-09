#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

void *thread_func(void *args)
{
    printf("%i started...\n", *(int *)args);
    for (size_t it = 0; it < 10000; it++) continue;
}

int main(int argc, char const *argv[])
{
    pthread_t p_th[5];
    for (int it = 0; it < 5; ++it) {
        int error_code;
        error_code = pthread_create(&p_th[it], NULL, thread_func, &it);
        if (error_code != 0) {
            errno = error_code;
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
    }
    for (int it = 0; it < 5; ++it) {
        pthread_join(p_th[it], NULL);
    }
    return 0;
}
