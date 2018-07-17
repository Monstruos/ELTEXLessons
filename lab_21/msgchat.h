// Определение всех постоянных, необходимых для работы и клиента и сервера, а
// также обертка для отправления и приема сообщений
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>

// Константы для определения типов сообщений
#define CLIENT_COUNT 5
#define CLIENT_LEVEL CLIENT_COUNT
#define CLIENT_SPACE (-CLIENT_LEVEL)
#define SERVER_LEVEL (CLIENT_LEVEL + 1)
#define SERVER_SPACE (-SERVER_LEVEL)


// Константы для определения статуса сообщений (положительные числа - новое
// сообщение от соответствующего клиента)
#define NEW_USER (-1)
#define ROOM_IS_FULL (-2)


// Константы для определения размеров полей структуры msgbuf
#define NAME_LENGTH 64
#define MESS_LENGTH 1024


// Переопределение структуры msgbuf
struct msgbuf {
    long mtype;
    int status;
    char name[NAME_LENGTH];
    char message[MESS_LENGTH];
};


// @send_in_queue: Отправить сообщение mess;
// id: идентификатор очереди сообщений;
// mess: сообщение.
void send_in_queue(int id, struct msgbuf *mess);


// @receive_from_queue: Получить сообщение из очереди и записать в mess;
// id: идентификатор очереди сообщений;
// mess: сообщение;
// type: Тип принимаемых сообщений.
void receive_from_queue(int id, struct msgbuf *mess, long type);
