#ifndef __FILE_MANAGER__H
#define __FILE_MANAGER__H


#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <limits.h>
#include "text_editor.h"
#include "load_win.h"


// Коды выхода из окна файлового менеджера
#define SWITCH_CODE 1
#define CLOSE_CODE 2


// @FILE_MANAGER: структура файлового менеджера;
// fm_window: окно файлового менеджера;
// parent: окно родительского окна;
// file_list: список файлов в текущей директории;
// dir_path: текущий абсолютный путь до директории;
// file_count: количество файлов в текущей директории;
// file_list_ptr: "указатель" на выбранный файл в текущей директории.
struct FILE_MANAGER {
    WINDOW *fm_window;
    WINDOW *parent;
    struct dirent **file_list;
    char dir_path[PATH_MAX];
    int file_count;
    int file_list_ptr;
};


// @init_file_manager_full_window: инициализация окна файлового менеджера.
// Возвращает указатель на структуру, описывающую созданное окно.
// parent: родительское окно. По нему определяются размеры окна файлового
// менеджера.
// note: родительское окно будет очищено и использоваться для обрамления
// файлового менеджера!
// note: менеджер открыт в "/"
struct FILE_MANAGER *init_file_manager_full_window(WINDOW *parent);


// @show_cur_dir_with_chosen_file: показ в окне файлового менеджера списка
// файлов директории, в которой находится файловый менеджер и выделяет
// выбранный элемент.
// fm: указатель на файловый менеджер.
int show_cur_dir_with_chosen_file(struct FILE_MANAGER *fm);


// @chosen_is_dir: логическая функция, возвращающая 1, если выбранный файл в
// текущей директории является директорией, иначе 0.
// fm: указатель на файловый менеджер.
int chosen_is_dir(struct FILE_MANAGER *fm);


// @chosen_is_exec: логическая функция, возвращающая 1, если выбранный файл в
// текущей директории является исполняемым, иначе 0.
// fm: указатель на файловый менеджер.
int chosen_is_exec(struct FILE_MANAGER *fm);


// @open_chosen_dir: открытие выбранной директории.
// fm: указатель на файловый менеджер.
// note: проверка на то, что выбранный файл - директория, не производится! для
// этого есть chosen_is_dir().
int open_chosen_dir(struct FILE_MANAGER *fm);


// @del_file_manager_window: удаление (освобождение памяти) файлового менеджера
// fm: указатель на файловый менеджер.
void del_file_manager_window(struct FILE_MANAGER *fm);


// @set_file_ptr: устанавливает указатель на файл в текущей директории на
// file_ptr позицию.
// fm: указатель на файловый менеджер.
// file_ptr: номер файла в списке директории. Проверка на
// file_ptr ~ [0, file_count) производится.
int set_file_ptr(struct FILE_MANAGER *fm, int file_ptr);


// @get_chosen_file_path: функция, возвращающая абсолютный путь до выбранного
// файла.
// fm: указатель на файловый менеджер.
char *get_chosen_file_path(struct FILE_MANAGER *fm);


// @open_dir: открывает директорию dir_name в файловом менеджере fm.
// fm: указатель на файловый менеджер.
// dir_name: путь до директории
int open_dir(struct FILE_MANAGER *fm, const char *dir_name);


// @close_curr_dir: подфункция очистки списка файлов в директории. В
// пользовательском коде встречаться не должна. Используется в
// del_file_manager_window().
// fm: указатель на файловый менеджер.
void close_curr_dir(struct FILE_MANAGER *fm);


// @fm_exec: функция-"логика" работы файлового менеджера. Реализует перемещение
// по списку файлов в директории, обработку нажатия клавиш. Возвращает код
// прерывания работы (переключение на другое окно (SWITCH_CODE), либо же
// закрытие программы (CLOSE_CODE)).
// fm: указатель на файловый менеджер.
int fm_exec(struct FILE_MANAGER *fm);


// @run_chosen_exec_file: запуск исполняемого файла, выбранного в файловом
// менеджере. Запуск без доп. параметров, очищает весь экран, после окончания
// возвращает окно к состоянию до запуска.
// fm: указатель на файловый менеджер.
void run_chosen_exec_file(struct FILE_MANAGER *fm);

// @is_directory: подфункция-проверка, является ли файл path директорией. В
// пользовательском коде встречаться не должна. Используется в chosen_is_dir().
// path: имя (путь до) файла.
int is_directory(const char *path);

// @is_exec: подфункция-проверка, является ли файл path исполняемым. В
// пользовательском коде встречаться не должна. Используется в
// chosen_is_exec().
// path: имя (путь до) файла.
int is_exec(const char *path);


#endif // __FILE_MANAGER__H
