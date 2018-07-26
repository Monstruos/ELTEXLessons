// Пример реализации многопроцессного UDP-сервера

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
// В данном заголовочном файле находится порт и размер буфера
#include "udp_server_client.h"


int main(int argc, char const *argv[])
{
    int sock_descr, sock_descr_new;
    struct sockaddr_in inet_server_sock;


    // Создание endpoint'а
    if ((sock_descr = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }


    // Заполнение структуры, описывающей сервер
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


    char buffer[BUF_SIZE];
    // Создание структуры информации о клиенте
    struct sockaddr_in inet_client_sock;
    socklen_t inet_client_sock_size = sizeof(inet_client_sock);
    while (1) {
        // Принятие сообщения от клиента, получение информации о нем
        if (recvfrom(sock_descr,
                     buffer,
                     BUF_SIZE,
                     0,
                     (struct sockaddr *) &inet_client_sock,
                     &inet_client_sock_size) == -1) {
            perror("Error from recvfrom()");
            exit(EXIT_FAILURE);
        }
        printf("Receive: %s\n", buffer);
        int sub_proc = fork();
        if (sub_proc == 0) {
            sock_descr_new = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (sock_descr_new < 0) {
                perror("socket() error");
                exit(EXIT_FAILURE);
            }
            strcat(buffer, ", client!");
            // Отправка сообщения клиенту
            if (sendto(sock_descr_new,
                       buffer,
                       BUF_SIZE,
                       0,
                       (struct sockaddr *) &inet_client_sock,
                       inet_client_sock_size) == -1) {
                perror("sendto() error");
                exit(EXIT_FAILURE);
            }
            printf("Send: %s\n", buffer);
            close(sock_descr_new);
            exit(EXIT_SUCCESS);
        }
    }
    close(sock_descr);
    return 0;
}
