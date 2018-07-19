#include "msgchat.h"


// Сервер чата на очереди сообщений. Нет обработки выхода клиента.
// Если будет время и смысл, доделаю


// Статусы клиентов
char USER_STATUS[CLIENT_COUNT];


// @chat_server: серверный обработчик сообщений клиентов;
// q_id: идентификатор очереди сообщений.
void chat_server(int q_id)
{
    struct msgbuf rcv_mess;
    struct msgbuf snd_mess;

    int new_user_it;
    long user_id;
    while (1) {
        receive_from_queue(q_id, &rcv_mess, SERVER_LEVEL);
        switch (rcv_mess.status) {
        case NEW_USER:
            if (rcv_mess.mtype != SERVER_LEVEL)
                break;

            // Поиск свободного места
            for (new_user_it = 0;
                 new_user_it < CLIENT_COUNT;
                 new_user_it++) {
                if (USER_STATUS[new_user_it] == 0) {
                    USER_STATUS[new_user_it] = 1;
                    snd_mess.status = NEW_USER;
                    snd_mess.mtype = (long) new_user_it + 1;
                    break;
                }
            }
            // Сообщение клиенту, что комната полна, если мест нет
            if (new_user_it == CLIENT_COUNT) {
                snd_mess.status = ROOM_IS_FULL;
                snd_mess.mtype = (long) new_user_it;
            }

            strncpy(snd_mess.name, rcv_mess.name, NAME_LENGTH);
            strncpy(snd_mess.message, "Hello!", MESS_LENGTH);

            send_in_queue(q_id, &snd_mess);
            printf("answer to new user[%s, type:%li, status:%i]\n",
                   snd_mess.name,
                   snd_mess.mtype,
                   snd_mess.status);
            break;
        default:
            // Сообщение есть
            if (errno != ENOMSG) {
                if (rcv_mess.mtype != SERVER_LEVEL)
                    break;
                printf("received from user[%s, type:%li, status:%i]: %s\n",
                       rcv_mess.name,
                       rcv_mess.mtype,
                       rcv_mess.status,
                       rcv_mess.message);
                user_id = rcv_mess.status;
                // Отправка сообщения всем остальным
                for (new_user_it = 0;
                     new_user_it < CLIENT_COUNT;
                     new_user_it++) {
                    if (user_id != (new_user_it + 1) &&
                        USER_STATUS[new_user_it] != 0) {
                        rcv_mess.mtype = (long)(new_user_it + 1);
                        send_in_queue(q_id, &rcv_mess);
                    }
                }
            }
            break;
        }
        rcv_mess.mtype = 0;
        rcv_mess.status = 0;
    }
}


int main(int argc, char const *argv[])
{
    key_t key = ftok("./Makefile", 'B');
    // Изначально чат пуст
    for (int i = 0; i < CLIENT_COUNT; ++i)
        USER_STATUS[i] = 0;
    int mesq_id = msgget(key, IPC_CREAT | 0666);
    if (mesq_id < 0) {
        perror("Key is not created");
        exit(EXIT_FAILURE);
    }

    chat_server(mesq_id);
    msgctl(mesq_id, IPC_RMID, 0);
    return 0;
}
