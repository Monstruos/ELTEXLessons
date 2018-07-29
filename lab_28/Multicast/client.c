// Пример реализации multicast-клиента

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


    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(struct ip_mreq));
    mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));


    struct sockaddr_in multicast;
    memset(&multicast, 0, sizeof(struct sockaddr_in));
    multicast.sin_family = AF_INET;
    multicast.sin_addr.s_addr = htonl(INADDR_ANY);
    multicast.sin_port = htons(PORT);
    int enable = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if (bind(sock, (struct sockaddr *) &multicast, sizeof(multicast)) == -1) {
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
