#include <string.h>
#include <stdlib.h>
#include <pthread.h>

// Длина строки ошибки
#define ERROR_STR_LENGTH 256


// @init_key: инициализация ключа
void init_key();

// @key_destructor: деструктор ключа. Используется в pthread_key_create.
void key_destructor(void *value);


// @errno(): Реентерабельная функция, ставящая в соотвествие коду ошибки строку
// об ошибке. Возвращает строку с содержанием ошибки (размер строки должен быть
// не менее ERROR_STR_LENGTH).
// note: за освобождение отвечает пользователь.
// error_code: указатель на код об ошибке
char *errno(int error_code);
