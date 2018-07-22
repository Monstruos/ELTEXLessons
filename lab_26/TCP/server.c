// Пример реализации TCP-сервера с очередью сообщений

// Использование очереди сообщений в потоках выглядит избыточно, но довольно
// удобно

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include "global_var.h"


// Длина сообщения и тип.
#define MESS_LENGTH 1024
#define MSG_TYPE 1
#define QUEUE_SIZE 5


/*
 * переопределение структуры msgbuf для передачи заявок между
 * процессами
 *  PARAMS : long mtype - тип сообщения (всегда должен быть MSG_TYPE)
 *           struct sockaddr_in client - информация о клиенте
 *           char message - сообщение клиента
 */
struct msgbuf {
    long mtype;
    int socket;
    char message[MESS_LENGTH];
};


// перечисление состояний обработчика: ожидание, чтение, обработка, отправка,
// ошибка
enum state {
    W_STATE,
    R_STATE,
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
void *handle_server_main(void *args);


int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in server, client;
    socklen_t cl_size = sizeof(client);
    char buffer[MESS_LENGTH];


    key_t key = ftok("./Makefile", 'A');
    int job_queue = msgget(key, IPC_CREAT | 0666);


    // Создание endpoint'а
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


    // Заполнение структуры, описывающей сервер
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server.sin_port = htons(PORT - 1);


    // Привязка к адресу
    if (bind(sock,
             (struct sockaddr *) &server,
             sizeof(server)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }


    pthread_t handler[NUM_OF_PROCESSES];
    for (size_t i = 0; i < NUM_OF_PROCESSES; i++) {
        if (pthread_create(&handler[i], NULL, handle_server_main, NULL) != 0) {
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
    }


    // "Прослушка"
    listen(sock, NUM_OF_PROCESSES);


    int it = 0;
    while (it < NUM_OF_REQUESTS) {
        it++;
        // "Привязка" клиента к дескриптору
        int sub_sock = accept(sock, (struct sockaddr *) &client, &cl_size);
        if (sub_sock == -1) {
            perror("accept() error");
            exit(EXIT_FAILURE);
        }


        // Принятие сообщения от клиента
        struct msgbuf job;
        job.mtype = MSG_TYPE;
        job.socket = sub_sock;
        send_in_queue(job_queue, &job);
    }
    for (size_t i = 0; i < NUM_OF_PROCESSES; i++) {
        if (pthread_join(handler[i], NULL) != 0) {
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
    }
    close(sock);
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


void *handle_server_main(void *args)
{
    key_t key = ftok("./Makefile", 'A');
    int job_queue = msgget(key, 0);
    enum state statement = W_STATE;
    struct msgbuf job;


    int it = 0;
    while (it < HANDLE_REQUESTS) {
        switch (statement) {
        case W_STATE:
            if (receive_from_queue(job_queue, &job, MSG_TYPE) == 0) {
                statement = R_STATE;
            } else {
                statement = E_STATE;
            }
            break;
        case R_STATE:
            if (recv(job.socket, job.message, MESS_LENGTH, 0) != -1) {
                statement = H_STATE;
            } else {
                statement = E_STATE;
            }
            break;
        case H_STATE:
            strcat(job.message, ", client!");
            statement = S_STATE;
            break;
        case S_STATE:
            if (send(job.socket, job.message, MESS_LENGTH, 0) != -1) {
                statement = W_STATE;
                close(job.socket);
                it++;
            } else {
                statement = E_STATE;
            }
            break;
        case E_STATE:
            perror("ERROR");
            statement = W_STATE;
            close(job.socket);
            it++;
            break;
        }
    }
    exit(EXIT_SUCCESS);
}
