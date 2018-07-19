// Клиент чата на очередях сообщений. Запуск: ./program <nickname>

#include <pthread.h>
#include "msgchat.h"


// Уровень пользователя (определяется join_chat()).
long CLIENT_ID;
// Имя клиента (параметр при запуске программы).
const char *CLIENT_NAME;


// @join_chat: отправка сообщения входа в чат;
// msg_id: идентификатор очереди сообщений;
// note: сообщение отправляется на уровне сервера, после чего ожидает отклика
// от сервера - информацию о типе - уровне пользователя, на котором клиент
// будет общаться с сервером.
void join_chat(int msg_id)
{
    struct msgbuf join_mess;
    struct msgbuf server_answer;
    join_mess.mtype = SERVER_LEVEL;
    join_mess.status = NEW_USER;
    strncpy(join_mess.name, CLIENT_NAME, NAME_LENGTH);
    send_in_queue(msg_id, &join_mess);
    receive_from_queue(msg_id, &server_answer, CLIENT_SPACE);
    // Сообщение адресовано клиенту
    if (strcmp(server_answer.name, CLIENT_NAME) == 0) {
        // Сообщение о разрешении присоединиться
        if (server_answer.status == NEW_USER) {
            CLIENT_ID = server_answer.mtype;
        } else {
            printf("Chat is not able now\n");
            exit(EXIT_FAILURE);
        }
    } else {
        send_in_queue(msg_id, &server_answer);
    }
    printf("You are joined to chat!\n");
}


void send_mess(int msg_id, char *mess)
{
    struct msgbuf send_mess;
    send_mess.mtype = SERVER_LEVEL;
    send_mess.status = CLIENT_ID;
    strncpy(send_mess.name, CLIENT_NAME, NAME_LENGTH);
    strncpy(send_mess.message, mess, MESS_LENGTH);
    send_in_queue(msg_id, &send_mess);
}


// @thread_rcv_func: поточная функция приема сообщений;
// args: аргументы поточной функции (передается идентификатор)
void *thread_rcv_func(void *args)
{
    struct msgbuf rcv_mess;
    int q_id = *(int *)args;
    while (1) {
        if (msgrcv(q_id,
                   (void *) &rcv_mess,
                   sizeof(struct msgbuf) - sizeof(long),
                   CLIENT_ID,
                   MSG_NOERROR | IPC_NOWAIT) == -1) {
            if (errno == EIDRM) {
               perror("Chat was closed");
               exit(EXIT_SUCCESS);
            }
            if (errno != ENOMSG) {
                perror("msgrcv() error");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("[%s]%s\n", rcv_mess.name, rcv_mess.message);
        }
    }
}


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        printf("Too low arguments\n");
        exit(EXIT_SUCCESS);
    }
    CLIENT_NAME = argv[1];
    key_t key = ftok("./Makefile", 'B');
    int mesq_id = msgget(key, 0);
    if (mesq_id < 0) {
        perror("Key is not created");
        exit(EXIT_FAILURE);
    }
    join_chat(mesq_id);
    pthread_t thread_id;
    int error_code;
    // Причин, почему идентификатор очереди не глобальная переменная, а
    // передается аргументом в функции, нет.
    error_code = pthread_create(&thread_id, NULL, thread_rcv_func, &mesq_id);
    if (error_code != 0) {
        errno = error_code;
        perror("pthread_create error");
        exit(EXIT_FAILURE);
    }
    char message[MESS_LENGTH];
    while(1) {
        scanf("%s", message);
        send_mess(mesq_id, message);
    }

    return 0;
}
