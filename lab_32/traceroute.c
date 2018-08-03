// Реализация tracroute. Запускается с указанием своего ip и ip назначения

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <sys/time.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>


#define ICMP_PAYLOAD 64
#define WAIT_TO_RECEIVE 5
#define PORT 65000

// Структура icmp пакета
struct icmp_packet {
    struct iphdr ip_h;
    struct icmphdr icmp_h;
    char payload[ICMP_PAYLOAD];
} __attribute__((packed));


/*
 * PURPOSE : Проверка, является ли пакет icmp на свой ip
 *  PARAMS : struct icmp_packet input - Входящий пакет
 *           unsigned long source - ip источника
 * RETURNS : int - Возвращает 1, в случае, если пакет нужный и типа reply или
 * time_exceeded; 2, если пакет типа destination_unreachable, иначе 0
 *   NOTES :
 */
int is_ping_packet(struct icmp_packet input, unsigned long source);
/*
 * PURPOSE : Реализация CRC-16
 *  PARAMS : uint16_t *start - Указатель на массив двухбайтных слов
 *           int word_num - количество слов
 * RETURNS : uint16_t - итоговая сумма
 *   NOTES :
 */
uint16_t checksum(uint16_t *start, int word_num);
/*
 * PURPOSE : Проверка, пришел ли пакет от точки назначения
 *  PARAMS : struct icmp_packet input - Входящий пакет
 *           unsigned long dest - ip назначения
 * RETURNS : int - 1, если пакет от точки назначения, иначе 0
 *   NOTES :
 */
int is_destination(struct icmp_packet input, unsigned long dest);


int main(int argc, char const *argv[])
{
    if (argc < 3) {
        printf("Usage with src ip and dest ip");
        exit(EXIT_SUCCESS);
    }
    unsigned long source = inet_addr(argv[1]);
    unsigned long dest = inet_addr(argv[2]);


    // Создание endpoint'а
    int sock;
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    int value = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &value, sizeof(value));


    // Заполнение структуры, описывающей сервер
    struct sockaddr_in server;
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = dest;
    server.sin_port = htons(PORT);


    // Заполнение ip заголовка
    struct iphdr send_ip;
    memset(&send_ip, 0, sizeof(struct iphdr));
    send_ip.version = 4;
    send_ip.ihl = 5;
    send_ip.ttl = 1;
    send_ip.protocol = IPPROTO_ICMP;
    send_ip.saddr = source;
    send_ip.daddr = dest;


    // Заполнение icmp заголовка
    struct icmphdr send_icmp;
    memset(&send_icmp, 0, sizeof(struct icmphdr));
    send_icmp.type = ICMP_ECHO;
    send_icmp.code = 0;
    send_icmp.checksum = checksum(&send_icmp,
        sizeof(struct icmphdr) / 2);


    // Формирование пакета
    struct icmp_packet ping;
    memset(ping.payload, 0, ICMP_PAYLOAD);
    ping.ip_h = send_ip;
    ping.icmp_h = send_icmp;


    // Время ожидания отклика
    struct timeval tv;
    tv.tv_sec = WAIT_TO_RECEIVE;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));


    // Последовательно отправляем icmp пакеты с увеличением ttl
    struct sockaddr_in point;
    socklen_t point_len = sizeof(point);
    char point_ip[INET_ADDRSTRLEN];
    struct icmp_packet input;
    int point_count = 1;
    do {
        // Отправка ping
        if (sendto(sock, &ping, sizeof(ping), 0,
            (struct sockaddr *) &server, sizeof(server)) == -1) {
            perror("sendto error");
            exit(EXIT_FAILURE);
        }


        // Принятие пакетов до тех пор, пока не придет пакет, нужный нам
        int check_res = 0;
        do {
            memset(&input, 0, sizeof(input));
            if (recvfrom(sock, &input, sizeof(input),
                0, &point, &point_len) == -1) {
                if (errno == EAGAIN) {
                    printf("Packet has no reply.\n");
                    break;
                }
                perror("recvfrom error");
                exit(EXIT_FAILURE);
            }
        } while ((check_res = is_ping_packet(input, source)) == 0);
        inet_ntop(AF_INET, &(point.sin_addr), point_ip, INET_ADDRSTRLEN);
        printf("%i. %s\n", point_count, point_ip);
        if (check_res == 2) {
            printf("Destination unreachable.\n");
            break;
        }
        point_count++;
        ping.ip_h.ttl++;
    } while (!is_destination(input, dest));


    close(sock);
    return 0;
}


int is_ping_packet(struct icmp_packet input, unsigned long source)
{
    if (input.ip_h.protocol == IPPROTO_ICMP && input.ip_h.daddr == source) {
        switch (input.icmp_h.type) {
        case ICMP_ECHOREPLY:
        case ICMP_TIME_EXCEEDED:
            return 1;
        case ICMP_DEST_UNREACH:
            return 2;
        }
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


int is_destination(struct icmp_packet input, unsigned long dest)
{
    if (input.ip_h.saddr == dest)
        return 1;
    return 0;
}
