// Пример реализации broadcast

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
    char buffer[BUF_SIZE] = "It's broadcast ping!";

    int enable_bc = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &enable_bc, sizeof(enable_bc));

    struct sockaddr_in bc_point;
    memset(&bc_point, 0, sizeof(struct sockaddr_in));
    bc_point.sin_family = AF_INET;
    bc_point.sin_addr.s_addr = htonl(INADDR_BROADCAST);
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
