#ifndef GLOBAL_VAR__H
#define GLOBAL_VAR__H


// Число обрабатывающих серверов
// #define NUM_OF_PROCESSES 2
// Число обрабатываемых заявок одним сервером
// #define HANDLE_REQUESTS 5
// Число заявок
#define NUM_OF_REQUESTS 40
// Порт для сокета.
#define PORT 48240
// Размер буфера
#define BUF_SIZE 512
// Размер udp заголовка
#define UDP_HEADER_LEN 8
// Размер ip заголовка
#define IP_HEADER_LEN 20
// Размер пакета
#define PACKET_SIZE (IP_HEADER_LEN + UDP_HEADER_LEN + BUF_SIZE)
// Размер ip заголовка в 4-байтных словах
#define IP_HEADER_WORD_LEN (IP_HEADER_LEN / 4)
// Первый байт заголовка ip (версия и размер заголовка в 4-байтный словах)
#define IP_V_AND_HEADER_LEN (64 + IP_HEADER_WORD_LEN)

#endif // GLOBAL_VAR__H
