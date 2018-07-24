// Пример реализации мультипротокольного сервера

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "global_var.h"

#include <sys/epoll.h>

#define EVENT_NUM 1

int main(int argc, char const *argv[])
{
    int tcp_sock, udp_sock;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t client_sock_size = sizeof(client);
    char buffer[BUF_SIZE];
    int epoll_fd;
    struct epoll_event event, revent[EVENT_NUM];


    // Создание endpoint'а
    if ((tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }


    if ((epoll_fd = epoll_create(2)) == -1) {
        perror("epoll error");
        exit(EXIT_FAILURE);
    }


    // Заполнение структуры, описывающей сервер
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);


    // Привязка к адресу
    if (bind(tcp_sock, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    if (bind(udp_sock, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }


    listen(tcp_sock, 1);


    event.events = EPOLLIN;
    event.data.fd = tcp_sock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_sock, &event) == -1) {
        perror("epoll_ctl (TCP)");
        exit(EXIT_FAILURE);
    }
    event.data.fd = udp_sock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_sock, &event) == -1) {
        perror("epoll_ctl (UDP)");
        exit(EXIT_FAILURE);
    }


    int it = 0;
    while (it < NUM_OF_REQUESTS) {
        it++;
        if (epoll_wait(epoll_fd, revent, EVENT_NUM, -1) == -1) {
            perror("poll error");
            exit(EXIT_FAILURE);
        }


        if (revent[0].data.fd == udp_sock) {
            if (recvfrom(udp_sock, buffer, BUF_SIZE, 0,
                (struct sockaddr *) &client, &client_sock_size) == -1) {
                perror("recvfrom error");
                exit(EXIT_FAILURE);
            }
            strcat(buffer, ", client!");
            if (sendto(udp_sock, buffer, BUF_SIZE, 0,
                (struct sockaddr *) &client, client_sock_size) == -1) {
                perror("sendto error");
                exit(EXIT_FAILURE);
            }
            printf("UDP handler (%i request)\n", it);
            continue;
        }

        if (revent[0].data.fd == tcp_sock) {
            int tcp_subsock;
            if ((tcp_subsock = accept(tcp_sock, (struct sockaddr *) &client,
                &client_sock_size)) < 0) {
                perror("accept error");
                exit(EXIT_FAILURE);
            }

            if (recv(tcp_subsock, buffer, BUF_SIZE, 0) == -1) {
                perror("recv error");
                exit(EXIT_FAILURE);
            }
            strcat(buffer, ", client!");
            if (send(tcp_subsock, buffer, BUF_SIZE, 0) == -1) {
                perror("send error");
                exit(EXIT_FAILURE);
            }
            shutdown(tcp_subsock, SHUT_RDWR);
            close(tcp_subsock);
            printf("TCP handler (%i request)\n", it);
            continue;
        }
    }
    close(udp_sock);
    close(tcp_sock);
    return 0;
}
