#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Данный набор функций позволяет создавать и работать с системой pipe'ов,
// используя массивы дескрипторов. Схема системы для 3 процессов:
// (STDIN_FILENO) -> [proc_1] --((1) pipe[0] (0))-> [proc_2] -->
// --((1) pipe[1] (0))-> [proc_3] -> (STDOUT_FILENO)

// @set_pipe_system: Создает pipe_count pipe'ов. Возвращает 0 в случае успеха,
// иначе 1;
// pipe_fd: указатель на двумерный массив файловых дескрипторов
// ([pipe_count][2]) - доступ к pipe. Память под массив выделяется в функции;
// pipe_count: количество создаваемых pipe'ов;
// note: пример запуска:
//     int proc_num = 3;
//     int **pipe_fd = NULL;
//     unsigned int pipe_count = proc_num - 1;
//     set_pipe_system(&pipe_fd, pipe_count);
int set_pipe_system(int ***pipe_fd, unsigned int pipe_count);


// @make_pipe_bridge: создает массив из 2 дескрипторов для процесса под номером
// ser_num ('input_fd' и 'output_fd', см. схему), закрывает все остальные
// дескрипторы в pipe_fd, ОСВОБОЖДАЕТ память из-под pipe_fd. Проверка на
// соответствие массива pipe_fd шаблону [pipe_count][2] не производится.
// Возвращает указатель на массив из 2 дескрипторов, иначе NULL.
// pipe_fd: двумерный массив файловых дескрипторов
// ([pipe_count][2]) - доступ к pipe;
// pipe_count: количество pipe'ов;
// ser_num: номер процесса;
// note: за освобождение возвращаемого массива отвечает программист.
int *make_pipe_bridge(int **pipe_fd, unsigned int pipe_count,
                      unsigned int ser_num);


#define CMD_BUFFER_SIZE 1024
#define ARGS_COUNT 10


// Работает только с командами без аргументов, проверялось на 'ls|cat'
int main(int argc, char *argv[])
{
    pid_t cpid;
    char cmd_str[CMD_BUFFER_SIZE] = "";
    char duplic_cmd_str[CMD_BUFFER_SIZE];
    while(1) {
        write(STDOUT_FILENO, "\n>", 2);
        scanf("%s", cmd_str);
        strcpy(duplic_cmd_str, cmd_str);
        if (strcmp(cmd_str, "exit_app") == 0)
            break;
        char *cmd_str_ptr = strtok(duplic_cmd_str, "|");
        unsigned int proc_num = 0;
        while (cmd_str_ptr != NULL) {
            proc_num++;
            cmd_str_ptr = strtok(NULL, "|");
        }

        int **pipe_fd = NULL;
        unsigned int pipe_count = proc_num - 1;
        set_pipe_system(&pipe_fd, pipe_count);
        strcpy(duplic_cmd_str, cmd_str);
        cmd_str_ptr = strtok(duplic_cmd_str, "|");
        for (size_t i = 0; i < proc_num; i++) {
            cpid = fork();
            if (cpid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (cpid == 0) {
                int *bridge = make_pipe_bridge(pipe_fd, pipe_count, i);
                dup2(bridge[0], STDIN_FILENO);
                dup2(bridge[1], STDOUT_FILENO);
                execlp(cmd_str_ptr, cmd_str_ptr, (char *) NULL);
                close(bridge[0]);
                close(bridge[1]);
                free(bridge);
                return 0;
            } else {
                if (i != 0)
                    wait(NULL);
                cmd_str_ptr = strtok(NULL, "|");
            }
        }
    }
    exit(EXIT_SUCCESS);
}

int set_pipe_system(int ***pipe_fd, unsigned int pipe_count)
{
    int **res_pipe_fd;
    res_pipe_fd = malloc(sizeof(int *) * pipe_count);
    for (size_t i = 0; i < pipe_count; i++) {
        res_pipe_fd[i] = malloc(sizeof(int) * 2);
        if (pipe(res_pipe_fd[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }
    *pipe_fd = res_pipe_fd;
    return 0;
}


int *make_pipe_bridge(int **pipe_fd, unsigned int pipe_count,
                      unsigned int ser_num)
{
    if (pipe_fd == NULL)
        return NULL;
    int *res_transit_fd = malloc(sizeof(int) * 2);

    // set first ('input') descriptor
    if (ser_num != 0)
        res_transit_fd[0] = pipe_fd[ser_num - 1][0];
    else
        res_transit_fd[0] = dup(STDIN_FILENO);

    // set second ('output') descriptor
    if (ser_num != pipe_count)
        res_transit_fd[1] = pipe_fd[ser_num][1];
    else
        res_transit_fd[1] = dup(STDOUT_FILENO);

    for (size_t i = 0; i < pipe_count; i++) {
        if (i != ser_num)
            close(pipe_fd[i][1]);
        if (i != ser_num - 1)
            close(pipe_fd[i][0]);
        free(pipe_fd[i]);
    }
    free(pipe_fd);
    return res_transit_fd;
}
