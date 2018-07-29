// Пример реализации UDP-клиента на raw-сокетах

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "global_var.h"


#define CLIENT_PORT 48241

struct udp_packet {
    uint16_t port_src;
    uint16_t port_dest;
    uint16_t length;
    uint16_t csum;
    char buffer[BUF_SIZE];
};


/*
 * PURPOSE : Проверка на то, принадлежит пакет серверу или нет
 *  PARAMS : struct sockaddr_in server - структура описания сервера
 *           struct udp_packet input - структура описания пакета
 * RETURNS : int - 1, если пакет от сервера, иначе 0
 *   NOTES :
 */
int is_server_packet(struct sockaddr_in server, struct udp_packet input);


int main(int argc, char const *argv[])
{
    int sock;
    // Создание endpoint'а
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    // Заполнение структуры, описывающей сервер
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);


    // "Упаковка" пакета
    struct udp_packet send_pack;
    memset(&send_pack, 0, sizeof(struct udp_packet));
    strcpy(send_pack.buffer, "HELLO");
    send_pack.port_src = htons(CLIENT_PORT);
    send_pack.port_dest = htons(PORT);
    send_pack.length = htons(BUF_SIZE + UDP_HEADER_LEN);
    send_pack.csum = 0;


    // Отправка сообщения серверу
    if (sendto(sock, &send_pack, sizeof(send_pack), 0,
        (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }


    struct udp_packet *sender_pack;
    char input[PACKET_SIZE];


    // Принимаются все udp пакеты, необходимо проверять источник и назначение
    do {
        memset(&input, 0, PACKET_SIZE);
        // Принятие сообщения от сервера
        if (recvfrom(sock, input, PACKET_SIZE,
            0, NULL, NULL) == -1) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }
        sender_pack = (struct udp_packet *) &(input[IP_HEADER_LEN]);
        printf("Packet received.\n");
    } while (!is_server_packet(server, *sender_pack));


    // Вывод payload'a, за ip и udp заголовком
    printf("Receive: %s\n", &(input[IP_HEADER_LEN + UDP_HEADER_LEN]));
    close(sock);
    return 0;
}


int is_server_packet(struct sockaddr_in server, struct udp_packet input)
{
    if ((input.port_src == server.sin_port) &&
        (input.port_dest == htons(CLIENT_PORT))) {
        return 1;
    }
    return 0;
}
