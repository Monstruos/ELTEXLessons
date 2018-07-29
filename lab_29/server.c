// Пример реализации эхо-сервера

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "global_var.h"

int main(int argc, char const *argv[])
{
    int udp_sock;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t client_sock_size = sizeof(client);
    char buffer[BUF_SIZE];


    // Создание endpoint'а
    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }


    // Заполнение структуры, описывающей сервер
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);


    // Привязка к адресу
    if (bind(udp_sock, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }


    if (recvfrom(udp_sock, buffer, BUF_SIZE, 0,
        (struct sockaddr *) &client, &client_sock_size) == -1) {
        perror("recvfrom error");
        exit(EXIT_FAILURE);
    }
    printf("Received: %s\n", buffer);


    strcat(buffer, ", client!");


    if (sendto(udp_sock, buffer, BUF_SIZE, 0,
        (struct sockaddr *) &client, client_sock_size) == -1) {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }


    close(udp_sock);
    return 0;
}
