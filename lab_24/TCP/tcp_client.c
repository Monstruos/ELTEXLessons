// Пример реализации TCP-клиента для эхо сервера

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
// В данном заголовочном файле находится порт и размер буфера
#include "tcp_server_client.h"


int main(int argc, char const *argv[])
{
    int sock_descr;
    // Создание endpoint'а
    sock_descr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Заполнение структуры, описывающей сервер
    struct sockaddr_in inet_server_sock;
    memset(&inet_server_sock, 0, sizeof(struct sockaddr_in));
    inet_server_sock.sin_family = AF_INET;
    inet_server_sock.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    inet_server_sock.sin_port = htons(PORT);

    // Подключение к серверу
    int res_conn = connect(sock_descr,
                           (struct sockaddr *) &inet_server_sock,
                           sizeof(inet_server_sock));
    if (res_conn == -1) {
        perror("connect() error");
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_SIZE];

    strcpy(buffer, "Hello");
    printf("Send: %s\n", buffer);
    // Отправка сообщения серверу
    if (send(sock_descr, buffer, BUF_SIZE, 0) == -1) {
        perror("send() error");
        exit(EXIT_FAILURE);
    }
    // Принятие сообщения от сервера
    if (recv(sock_descr, buffer, BUF_SIZE, 0) == -1) {
        perror("recv() error");
        exit(EXIT_FAILURE);
    }
    printf("Receive: %s\n", buffer);
    close(sock_descr);
    return 0;
}
