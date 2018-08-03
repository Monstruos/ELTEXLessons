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


struct udp_header {
    uint16_t port_src;
    uint16_t port_dest;
    uint16_t length;
    uint16_t csum;
};

struct ip_header {
    uint8_t v_head_len;
    uint8_t tos;
    uint16_t to_len;
    uint16_t ident;
    uint16_t flags_offset;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t src_ip;
    uint32_t dest_ip;
};

struct packet {
    struct ip_header ip_h;
    struct udp_header udp_h;
    char payload[BUF_SIZE];
};

/*
 * PURPOSE : Проверка на то, принадлежит пакет серверу или нет
 *  PARAMS : struct sockaddr_in server - структура описания сервера
 *           struct udp_header input - структура описания пакета
 * RETURNS : int - 1, если пакет от сервера, иначе 0
 *   NOTES :
 */
int is_server_packet(struct sockaddr_in server, struct udp_header input);


int main(int argc, char const *argv[])
{
    char packet[PACKET_SIZE];
    int sock;
    // Создание endpoint'а
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    int value = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &value, sizeof(value));


    // Заполнение структуры, описывающей сервер
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);


    // Заполнение ip заголовка
    struct ip_header *send_ip = (struct ip_header *) packet;
    memset(send_ip, 0, sizeof(struct ip_header));
    send_ip->v_head_len = IP_V_AND_HEADER_LEN;
    send_ip->ttl = 2;
    send_ip->proto = 17;
    send_ip->src_ip = inet_addr("127.0.0.1");
    send_ip->dest_ip = inet_addr("127.0.0.1");


    // Заполнение udp заголовка
    struct udp_header *send_udp =
        (struct udp_header *) &(packet[IP_HEADER_LEN]);
    memset(send_udp, 0, sizeof(struct udp_header));
    send_udp->port_src = htons(CLIENT_PORT);
    send_udp->port_dest = htons(PORT);
    send_udp->length = htons(BUF_SIZE + UDP_HEADER_LEN);
    send_udp->csum = 0;


    // Сообщение
    strcpy(&(packet[IP_HEADER_LEN + UDP_HEADER_LEN]), "HELLO");


    // Отправка сообщения серверу
    if (sendto(sock, packet, PACKET_SIZE, 0,
        (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }


    struct udp_header *sender_pack;
    char input[PACKET_SIZE];


    // Принятие пакетов до тех пор, пока не придет пакет, принадлежащий нам
    do {
        memset(input, 0, PACKET_SIZE);
        // Принятие сообщения от сервера
        if (recvfrom(sock, input, PACKET_SIZE,
            0, NULL, NULL) == -1) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }
        sender_pack = (struct udp_header *) &(input[IP_HEADER_LEN]);
        printf("Packet received.\n");
    } while (!is_server_packet(server, *sender_pack));


    printf("Receive: %s\n", &(input[IP_HEADER_LEN + UDP_HEADER_LEN]));
    close(sock);
    return 0;
}


int is_server_packet(struct sockaddr_in server, struct udp_header input)
{
    if ((input.port_src == server.sin_port) &&
        (input.port_dest == htons(CLIENT_PORT))) {
        return 1;
    }
    return 0;
}
