// Реализация ping. Запускается с указанием своего ip и ip назначения

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
 * RETURNS : int - Возвращает 1, в случае, если пакет нужный, иначе 0
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


    // Самостоятельная генерация ip заголовка
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
    send_ip.ttl = 64;
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


    for (size_t i = 0; i < 5; i++) {
        // Отправка ping
        if (sendto(sock, &ping, sizeof(ping), 0,
            (struct sockaddr *) &server, sizeof(server)) == -1) {
            perror("sendto error");
            exit(EXIT_FAILURE);
        }


        // Принятие пакетов до тех пор, пока не придет пакет, нужный нам
        struct icmp_packet input;
        do {
            memset(&input, 0, sizeof(input));
            if (recvfrom(sock, &input, sizeof(input),
                0, NULL, NULL) == -1) {
                if (errno == EAGAIN) {
                    printf("Packet has no reply.\n");
                    break;
                }
                perror("recvfrom error");
                exit(EXIT_FAILURE);
            }
        } while (!is_ping_packet(input, source));
    }
    close(sock);
    return 0;
}


int is_ping_packet(struct icmp_packet input, unsigned long source)
{
    if (input.ip_h.protocol == IPPROTO_ICMP) {
        if (input.ip_h.daddr == source) {
            if (input.icmp_h.type == ICMP_ECHOREPLY) {
                printf("Ping received.\n");
                return 1;
            }
            if (input.icmp_h.type == ICMP_DEST_UNREACH) {
                printf("Destination unreachable.\n");
                return 1;
            }
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
