// Пример реализации TCP-сервера для многопроцессного сервера (на основе эхо-
// сервера)

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

#define QUEUE_SIZE 5


int main(int argc, char const *argv[])
{
    int sock_descr;
    // Создание endpoint'а
    sock_descr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Заполнение структуры, описывающей сервер
    struct sockaddr_in inet_server_sock;
    memset(&inet_server_sock, 0, sizeof(struct sockaddr_in));
    inet_server_sock.sin_family = AF_INET;
    inet_server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
    inet_server_sock.sin_port = htons(PORT);
    // Привязка к адресу
    if (bind(sock_descr,
             (struct sockaddr *) &inet_server_sock,
             sizeof(inet_server_sock)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    // "Прослушка"
    listen(sock_descr, QUEUE_SIZE);

    // Создание структуры информации о клиенте
    struct sockaddr_in inet_client_sock;
    socklen_t inet_client_sock_size = sizeof(inet_client_sock);
    char buffer[BUF_SIZE];
    int sub_sock_descr;
    while (1) {
        // "Привязка" клиента к дескриптору
        sub_sock_descr = accept(sock_descr,
            (struct sockaddr *) &inet_client_sock,
            &inet_client_sock_size);

        if (sub_sock_descr < 0) {
            perror("accept()");
            exit(EXIT_FAILURE);
        }
        int sub_proc = fork();
        if (sub_proc == 0) {
            // Принятие сообщения от клиента
            if (recv(sub_sock_descr, buffer, BUF_SIZE, 0) == -1) {
                perror("recv() error");
                exit(EXIT_FAILURE);
            }
            printf("Receive: %s\n", buffer);
            strcat(buffer, ", client!");
            printf("Send: %s\n", buffer);
            // Отправка сообщения клиенту
            if (send(sub_sock_descr, buffer, BUF_SIZE, 0) == -1) {
                perror("send() error");
                exit(EXIT_FAILURE);
            }
            sleep(10);
            close(sub_sock_descr);
            exit(EXIT_SUCCESS);
        }
;
    }
    close(sock_descr);
    return 0;
}
