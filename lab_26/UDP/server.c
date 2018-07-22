// Пример реализации TCP-сервера с очередью. Основан на эхо сервере.


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "global_var.h"


// Длина сообщения и тип.
#define MESS_LENGTH 1024
#define MSG_TYPE 1


/*
 * переопределение структуры msgbuf для передачи заявок между
 * процессами
 *  PARAMS : long mtype - тип сообщения (всегда должен быть MSG_TYPE)
 *           struct sockaddr_in client - информация о клиенте
 *           char message - сообщение клиента
 */
struct msgbuf {
    long mtype;
    struct sockaddr_in client;
    char message[MESS_LENGTH];
};


// перечисление состояний обработчика: ожидание, обработка, отправка и ошибка
enum state {
    W_STATE,
    H_STATE,
    S_STATE,
    E_STATE
};


/*
 * PURPOSE : Отправляет сообщения в очередь сообщений
 *  PARAMS : int id - идентификатор очереди
 *           struct msgbuf *mess - указатель на сообщение
 * RETURNS : int - 0 в случае успеха, иначе 1
 *   NOTES :
 */
int send_in_queue(int id, struct msgbuf *mess);
/*
 * PURPOSE : Получение сообщения из очереди
 *  PARAMS : int id - идентификатор очереди
 *           struct msgbuf *mess - указатель на структуру, в которую будет
 *           записано сообщение
 *           long type - тип сообщение (в данной программе всегда MSG_TYPE)
 * RETURNS : int - 0 в случае успеха, иначе 1
 *   NOTES :
 */
int receive_from_queue(int id, struct msgbuf *mess, long type);
/*
 * PURPOSE : Функция обработки заявок клиентов
 *  PARAMS : int sock - дескриптор сокета
 *           int num - номер процесса (необходим исключительно для
 *           показа работы)
 * RETURNS : None
 *   NOTES : работает по принципу конечного автомата
 */
void handle_server_main(int sock, int num);


int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in server;
    struct sockaddr_in client_sock;
    socklen_t client_sock_size = sizeof(client_sock);


    key_t key = ftok("./Makefile", 'A');
    int job_queue = msgget(key, IPC_CREAT | 0666);


    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


    // Заполнение структуры, описывающей сервер
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);


    // Привязка к адресу
    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }


    for (size_t i = 0; i < NUM_OF_PROCESSES; i++) {
        if (!fork()) {
            handle_server_main(sock, i);
        }
    }

    int it = 0;
    while (it < NUM_OF_REQUESTS) {
        it++;
        struct msgbuf input_job;
        if (recvfrom(sock, input_job.message, MESS_LENGTH, 0,
            (struct sockaddr *) &client_sock, &client_sock_size) == -1) {
            perror("Error from recvfrom()");
            exit(EXIT_FAILURE);
        }
        input_job.mtype = MSG_TYPE;
        input_job.client = client_sock;
        send_in_queue(job_queue, &input_job);
    }


    close(sock);
    wait(NULL);
    msgctl(job_queue, IPC_RMID, 0);
    return 0;
}


int send_in_queue(int id, struct msgbuf *mess)
{
    if (msgsnd(id, (void *) mess, sizeof(struct msgbuf) - sizeof(long),
        IPC_NOWAIT) == -1)
        return 1;
    return 0;
}


int receive_from_queue(int id, struct msgbuf *mess, long type)
{
    if (msgrcv(id, (void *) mess, sizeof(struct msgbuf) - sizeof(long),
        type, MSG_NOERROR) == -1)
       return 1;
    return 0;
}


void handle_server_main(int new_sock, int num)
{
    key_t key = ftok("./Makefile", 'A');
    int job_queue = msgget(key, 0);
    enum state statement = W_STATE;
    struct msgbuf job;

    int it = 0;
    while (it < HANDLE_REQUESTS) {
        switch (statement) {
        case W_STATE:
            receive_from_queue(job_queue, &job, MSG_TYPE);
            statement = H_STATE;
            break;
        case H_STATE:
            strcat(job.message, ", client!");
            statement = S_STATE;
            break;
        case S_STATE:
            if (sendto(new_sock, job.message, MESS_LENGTH, 0,
                (struct sockaddr *) &job.client, sizeof(job.client)) == -1) {
                statement = E_STATE;
            } else {
                statement = W_STATE;
                it++;
                printf("handle %i\n", num);
            }
            break;
        case E_STATE:
            perror("ERROR");
            statement = W_STATE;
            it++;
            break;
        }
    }
    exit(EXIT_SUCCESS);
}
