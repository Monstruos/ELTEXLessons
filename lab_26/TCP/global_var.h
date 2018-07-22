#ifndef GLOBAL_VAR__H
#define GLOBAL_VAR__H


// Число обрабатывающих серверов
#define NUM_OF_PROCESSES 5
// Число обрабатываемых заявок одним сервером
#define HANDLE_REQUESTS 2000
// Число заявок
#define NUM_OF_REQUESTS (NUM_OF_PROCESSES * HANDLE_REQUESTS)
// Порт для сокета.
#define PORT 48240



#endif // GLOBAL_VAR__H
