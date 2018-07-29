#ifndef GLOBAL_VAR__H
#define GLOBAL_VAR__H


// Число обрабатывающих серверов
// #define NUM_OF_PROCESSES 2
// Число обрабатываемых заявок одним сервером
// #define HANDLE_REQUESTS 5
// Число заявок
// #define NUM_OF_REQUESTS 40
// Порт для сокета.
#define PORT 48240
// Размер буфера
#define BUF_SIZE 512
// Длина udp заголовка
#define UDP_HEADER_LEN 8
// Длина ip заголовка
#define IP_HEADER_LEN 20
// Размер udp пакета
#define PACKET_SIZE (IP_HEADER_LEN + UDP_HEADER_LEN + BUF_SIZE)

#endif // GLOBAL_VAR__H
