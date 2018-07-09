#include <string.h>
#include <stdlib.h>
#include <pthread.h>

// Длина строки ошибки
#define ERROR_STR_LENGTH 256


// Инициализация ключа
void init_key();

// Деструктор ключа
void key_destructor();

/*
 * errno(): Реентерабельная функция, ставящая в соотвествие коду ошибки строку
 * об ошибке
 * error_code: указатель на код об ошибке
 * return: строка с содержанием ошибки (размер строки должен быть не менее
 * ERROR_STR_LENGTH), за освобождение отвечает пользователь.
 */
char *errno(int error_code);
