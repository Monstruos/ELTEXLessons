// Пример реализации UDP-сервера для эхо сервера

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
// В данном заголовочном файле находится порт и размер буфера
#include "udp_server_client.h"


int main(int argc, char const *argv[])
{
    int sock_descr;
    // Создание endpoint'а
    sock_descr = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Заполнение структуры, описывающей сервер
    struct sockaddr_in inet_server_sock;
    memset(&inet_server_sock, 0, sizeof(struct sockaddr_in));
    inet_server_sock.sin_family = AF_INET;
    inet_server_sock.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
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
    strcat(buffer, ", client!");
    // Отправка сообщения клиенту
    if (sendto(sock_descr,
               buffer,
               BUF_SIZE,
               0,
               (struct sockaddr *) &inet_client_sock,
               inet_client_sock_size) == -1) {
        perror("sendto() error");
        exit(EXIT_FAILURE);
    }
    printf("Send: %s\n", buffer);
    close(sock_descr);
    return 0;
}
