#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

// Размер блока для копирования (чем больше, тем быстрее).
#define BLOCK_SIZE 1

// @load_win_info: Информация для создания и работы окна прогресса (в отдельном
// потоке);
// parent_win: указатель на окно, в котором будет создано окно прогресса
// функциями);
// status: указатель на переменную, определяющую прогресс действия (в случае с
// load_win_thread: дескриптор потока с окном прогресса (инициализируется
// работой с файлом может определяться количеством считанных/записанных байт);
// size: максимально возможное значение status.
struct load_win_info
{
    WINDOW *parent_win;
    pthread_t load_win_thread;
    int *status;
    int size;
};

// @init_load_window: запускает в отдельном потоке окно прогресса;
// info: параметры для окна прогресса;
// #: после окончания работы окна родительское окно будет очищено.
void init_load_window(struct load_win_info *info);

// @load_window_sync: ожидает окончания работы окна прогресса. Необходимо для
// гарантированного ощичения экрана от окна прогресса;
// info: параметры для окна прогресса.
void load_window_sync(struct load_win_info *info);

// @load_thread_func: функция изменения окна прогресса. Данная функция
// запускается в init_load_window();
// args: параметры для окна прогресса. Необходимо передавать указатель на
// структуру load_win_info.
void *load_thread_func(void *args);

// @copy_file_with_status: функция копирования из одного файла в другой с
// отображением прогресса;
// init_window: родительское окно окна прогресса. Размер окна прогресса определяется размерами init_window.
//     note: init_window будет очищено после окончания работы функции!
// orig_file_path: путь (имя) копируемого файла;
// copy_file_path: путь до файла (имя файла), в который копируется содержимое
//     note: файл будет перезаписан.
void copy_file_with_status(WINDOW *init_window,
                           char *orig_file_path,
                           char *copy_file_path);

// @open_request_copy_file: открывает окно запроса имени файла, в который будет
// скопирован файл. Возвращает то, что ввел пользователь (проверок на то, что
// это имя (путь до) файла, нет) - строка размером PATH_MAX. Строка не
// освобождается автоматически;
// init_window: родительское окно, на основе размеров которого будет построено
// окно запроса. После окончания ввода будет очищено.
char *open_request_copy_file(WINDOW *init_window);
