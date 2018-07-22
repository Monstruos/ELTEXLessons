// Пример реализации TCP-клиента

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "global_var.h"


#define BUF_SIZE 1024


int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in server;
    char buffer[BUF_SIZE];
    int requests = NUM_OF_REQUESTS;


    if (argc > 1)
        requests = atoi(argv[1]);


    // Заполнение структуры, описывающей сервер
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server.sin_port = htons(PORT - 1);


    int it = 0;
    while (it < requests) {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        // Подключение к серверу
        if (connect(sock, (struct sockaddr *) &server, sizeof(server)) == -1) {
            printf("!%i!\n", it);
            perror("connect() error");
            exit(EXIT_FAILURE);
        }


        strcpy(buffer, "Hello");
        // Отправка сообщения серверу
        if (send(sock, buffer, BUF_SIZE, 0) == -1) {
            printf("!%i!\n", it);
            perror("send() error");
            exit(EXIT_FAILURE);
        }
        if (!fork()) {
            // Принятие сообщения от сервера
            if (recv(sock, buffer, BUF_SIZE, 0) == -1) {
                printf("!%i!\n", it);
                perror("recv() error");
                exit(EXIT_FAILURE);
            }
            printf("%i: Receive: %s\n", it, buffer);
            close(sock);
            exit(EXIT_SUCCESS);
        }
        close(sock);
        it++;
    }
    return 0;
}
