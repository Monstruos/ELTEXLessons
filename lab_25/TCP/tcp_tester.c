// Реализация множества TCP-клиентов для многопроцессного сервера (на основе
// эхо-сервера)

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
// В данном заголовочном файле находится порт и размер буфера
#include "tcp_server_client.h"

int main(int argc, char const *argv[])
{
    struct sockaddr_in inet_server_sock;
    int sock_descr;
    char buffer[BUF_SIZE];
    strcpy(buffer, "Hello");
    int it = 0;
    while (1) {
        it++;
        // Создание endpoint'а
        if ((sock_descr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
            perror("socket() error");
            printf("%i\n\n", it);
            exit(EXIT_FAILURE);
        }

        // Заполнение структуры, описывающей сервер
        memset(&inet_server_sock, 0, sizeof(struct sockaddr_in));
        inet_server_sock.sin_family = AF_INET;
        inet_server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
        inet_server_sock.sin_port = htons(PORT);
        // Подключение к серверу
        int res_conn = connect(sock_descr,
                               (struct sockaddr *) &inet_server_sock,
                               sizeof(inet_server_sock));
        if (res_conn == -1) {
            perror("connect() error");
            printf("%i\n\n", it);
            exit(EXIT_FAILURE);
        }
        // Отправка сообщения серверу
        if (send(sock_descr, buffer, BUF_SIZE, 0) == -1) {
            perror("send() error");
            printf("%i\n\n", it);
            exit(EXIT_FAILURE);
        }
        if (!fork()) {
            // Принятие сообщения от сервера
            if (recv(sock_descr, buffer, BUF_SIZE, 0) == -1) {
                perror("recv() error");
                exit(EXIT_FAILURE);
            }
            sleep(20);
            close(sock_descr);
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
