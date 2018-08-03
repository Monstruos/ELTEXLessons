// Реализация udp-клиента для эхо-сервера на raw-сокетах на канальном уровне

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "global_var.h"


// Порт клиента
#define CLIENT_PORT 48241

// Cтруктура udp заголовка
struct udp_header {
    uint16_t port_src;
    uint16_t port_dest;
    uint16_t length;
    uint16_t csum;
};

// Структура пакета
struct packet {
    struct ethhdr eth_h;
    struct iphdr ip_h;
    struct udp_header udp_h;
    char payload[BUF_SIZE];
} __attribute__((packed));

/*
 * PURPOSE : Проверка на то, принадлежит пакет серверу или нет
 *  PARAMS : struct sockaddr_in server - структура описания сервера
 *           struct udp_header input - структура описания пакета
 * RETURNS : int - 1, если пакет от сервера, иначе 0
 *   NOTES :
 */
int is_server_packet(struct packet input);
/*
 * PURPOSE : Реализация CRC-16
 *  PARAMS : uint16_t *start - Начало отсчета
 *           int word_num - количество считываемых слов
 * RETURNS : uint16_t - результат суммы
 *   NOTES :
 */
uint16_t checksum(uint16_t *start, int word_num);
/*
 * PURPOSE : Перевод строки с mac-адресом в массив чисел
 *  PARAMS : const char *str - Входящая строка с mac-адресом
 *           unsigned char *mac - Исходящие числа
 * RETURNS : int - код результата
 *   NOTES : числа реализованы через unsigned char
 */
int str_to_mac(const char *str, unsigned char *mac);


int main(int argc, char const *argv[])
{
    if (argc < 6) {
        printf("Usage with 5 args[src ip, mac; dest ip, mac; int]");
        return 0;
    }


    // Создание endpoint'а
    int sock;
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));


    // Заполнение структуры, описывающей сервер
    struct sockaddr_ll server;
    memset(&server, 0, sizeof(struct sockaddr_ll));
    server.sll_family = AF_PACKET;
    server.sll_ifindex = if_nametoindex(argv[5]);
    server.sll_halen = ETH_ALEN;
    server.sll_protocol = ETH_P_IP;
    str_to_mac(argv[4], server.sll_addr);


    // Заполнение eth заголовка
    struct ethhdr send_eth;
    memset(&send_eth, 0, sizeof(struct ethhdr));
    str_to_mac(argv[2], send_eth.h_source);
    str_to_mac(argv[4], send_eth.h_dest);
    send_eth.h_proto = htons(ETH_P_IP);


    // Заполнение ip заголовка
    struct iphdr send_ip;
    memset(&send_ip, 0, sizeof(struct iphdr));
    send_ip.version = 4;
    send_ip.ihl = 5;
    send_ip.tot_len = htons(sizeof(struct packet) - sizeof(struct ethhdr));
    send_ip.ttl = 255;
    send_ip.protocol = IPPROTO_UDP;
    send_ip.saddr = inet_addr(argv[1]);
    send_ip.daddr = inet_addr(argv[3]);
    send_ip.id = rand();
    send_ip.check = checksum(&send_ip,  sizeof(struct iphdr) / 2);


    // Заполнение udp заголовка
    struct udp_header send_udp;
    memset(&send_udp, 0, sizeof(struct udp_header));
    send_udp.port_src = htons(CLIENT_PORT);
    send_udp.port_dest = htons(SERVER_PORT);
    send_udp.length = htons(BUF_SIZE + UDP_HEADER_LEN);
    send_udp.csum = 0;


    // Формирование пакета
    struct packet packet;
    memset(&packet, 0, sizeof(struct packet));
    strcpy(packet.payload, "HELLO");
    packet.ip_h = send_ip;
    packet.udp_h = send_udp;
    packet.eth_h = send_eth;


    // Отправка сообщения серверу
    if (sendto(sock, &packet, sizeof(packet), 0,
        (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("sendto error");
        exit(EXIT_FAILURE);
    }


    // Принятие пакетов до тех пор, пока не придет пакет, принадлежащий нам
    struct packet input;
    do {
        memset(&input, 0, sizeof(input));
        // Принятие сообщения от сервера
        if (recvfrom(sock, &input, sizeof(input),
            0, NULL, NULL) == -1) {
            perror("recvfrom error");
            exit(EXIT_FAILURE);
        }
        printf("Packet received.\n");
    } while (!is_server_packet(input));


    printf("Receive: %s\n", input.payload);
    close(sock);
    return 0;
}


int is_server_packet(struct packet input)
{
    if (input.eth_h.h_proto == htons(ETH_P_IP))
        if ((input.udp_h.port_src == htons(SERVER_PORT)) &&
            (input.udp_h.port_dest == htons(CLIENT_PORT))) {
            return 1;
        }
    return 0;
}

uint16_t checksum(uint16_t *start, int word_num)
{
    unsigned long buffer = 0;
    for (size_t i = 0; i < word_num; i++, start++)
        buffer += *start;
    buffer = (buffer >> 16) + (buffer & 0xFFFF);
    buffer += (buffer >> 16);
    return (uint16_t) (~buffer);
}

int str_to_mac(const char *str, unsigned char *mac)
{
    sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%*c",
    &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

    return 0;
}
