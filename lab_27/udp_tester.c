// Реализация множества UDP-клиентов для сервера (на основе эхо-сервера)

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "global_var.h"

int main(int argc, char const *argv[])
{
    struct sockaddr_in inet_server_sock;
    int sock_descr;
    char buffer[BUF_SIZE];
    strcpy(buffer, "Hello");


    int request_num = NUM_OF_REQUESTS;
    if (argc > 1)
        request_num = atoi(argv[1]);
    int it = 0;
    while (it < request_num) {
        // Создание endpoint'а
        it++;
        if ((sock_descr = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
            perror("socket() error");
            exit(EXIT_FAILURE);
        }

        // Заполнение структуры, описывающей сервер
        memset(&inet_server_sock, 0, sizeof(struct sockaddr_in));
        inet_server_sock.sin_family = AF_INET;
        inet_server_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
        inet_server_sock.sin_port = htons(PORT);
        // Отправка сообщения серверу
        if (sendto(sock_descr, buffer, BUF_SIZE, 0,
            (struct sockaddr *) &inet_server_sock,
            sizeof(inet_server_sock)) == -1) {
            perror("sendto() error");
            exit(EXIT_FAILURE);
        }
        if (!fork()) {
            // Принятие сообщения от сервера
            if (recvfrom(sock_descr, buffer, BUF_SIZE, 0, NULL, NULL) == -1) {
                perror("Error from recvfrom()");
                exit(EXIT_FAILURE);
            }
            printf("Request %i handled: %s\n", it, buffer);
            sleep(20);
            close(sock_descr);
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
