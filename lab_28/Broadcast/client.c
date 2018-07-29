// Пример реализации broadcast-клиента

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
    int sock;
    char buffer[BUF_SIZE];

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in broadcast;
    memset(&broadcast, 0, sizeof(struct sockaddr_in));
    broadcast.sin_family = AF_INET;
    broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcast.sin_port = htons(PORT);
    int enable = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
    // Для запуска другого клиента
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if (bind(sock, (struct sockaddr *) &broadcast, sizeof(broadcast)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < NUM_OF_REQUESTS; i++) {
        if (recvfrom(sock, buffer, BUF_SIZE, 0, NULL, NULL) == -1) {
            perror("Error from recvfrom()");
            exit(EXIT_FAILURE);
        }
        printf("Receive: %s\n", buffer);
    }

    close(sock);
    return 0;
}
