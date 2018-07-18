#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void sig_handler(int sig)
{
    printf("Hello, it's signal handler!\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    signal(SIGUSR1, sig_handler);
    while (1) continue;
    return 0;
}
