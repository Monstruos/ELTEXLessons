// Пример реализации multicast

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
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    char buffer[BUF_SIZE] = "It's multicast ping!";

    struct sockaddr_in bc_point;
    memset(&bc_point, 0, sizeof(struct sockaddr_in));
    bc_point.sin_family = AF_INET;
    bc_point.sin_addr.s_addr = inet_addr("224.0.0.1");
    bc_point.sin_port = htons(PORT);


    for (size_t i = 0; i < NUM_OF_REQUESTS; i++) {
        if (sendto(sock, buffer, BUF_SIZE, 0, (struct sockaddr *) &bc_point,
            sizeof(bc_point)) == -1) {
            perror("sendto() error");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    close(sock);
    return 0;
}
