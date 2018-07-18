#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void sig_handler(int sig)
{
    printf("Hello, it's SIGUSR handler!\n");
}

int main(int argc, char const *argv[])
{
    struct sigaction new_act, old_act;
    new_act.sa_handler = sig_handler;
    new_act.sa_flags = 0;
    sigfillset(&new_act.sa_mask);
    sigaction(SIGUSR1, &new_act, &old_act);
    while (1) {
        pause();
        printf("main: signal is handled!\n");
    }
    return 0;
}
