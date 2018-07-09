#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void print_id(int num)
{
    printf("pid_%i | pid: %i, ppid: %i\n", num, getpid(), getppid());
}


int main(int argc, char const *argv[])
{
    // pid_1 is main process
    pid_t pid_2, pid_3;
    if ((pid_2 = fork()) == 0) {
        pid_t pid_4;
        if ((pid_4 = fork()) == 0) {
            print_id(4);
        } else {
            wait(0);
            print_id(2);
        }
    } else {
        if ((pid_3 = fork()) == 0) {
            pid_t pid_5, pid_6;
            if ((pid_5 = fork()) == 0) {
                print_id(5);
            } else {
                if ((pid_6 = fork()) == 0) {
                    print_id(6);
                } else {
                    wait(0);
                    print_id(3);
                }
            }
        } else {
            wait(0);
            print_id(1);
        }
    }
    return 0;
}
