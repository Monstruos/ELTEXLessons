#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <limits.h>

// Максимальный размер открываемого файла.
#define MAX_SIZE_OF_TEXT 65536

// @init_texteditor_full_window: инициализация окна текстового редактора внутри
// родительского окна;
// note: содержимое родительского окна будет перезаписано;
// parent: указатель на родительское окно.
WINDOW *init_texteditor_full_window(WINDOW *parent);


// @run_texteditor: запуск текстового редактора;
// editor_win: указатель на окно текстового редактора.
void run_texteditor(WINDOW *editor_win);


// @find_and_open_file_in_editor: открывает окно запроса имени файла для
// открытия и открывает его в редакторе;
// editor_win: указатель на окно текстового редактора. По нему окно запроса
// определяет свои размеры.
void find_and_open_file_in_editor(WINDOW *editor_win);


// @open_and_save_file_in_editor: открывает окно запроса имени файла для
// сохранения. Если файл существует, он будет перезаписан, иначе будет создан
// новый. Если по каким-то причинам создание файла невозможно, будет выведено
// сообщение об ошибке;
// editor_win: указатель на окно текстового редактора. По нему окно запроса
// определяет свои размеры.
void open_and_save_file_in_editor(WINDOW *editor_win);


// @open_file_in_editor: открывает файл file_name в текстовом редакторе. При
// ошибке при открытии файла окно текстового редактора будет пустым.
// editor_win: указатель на окно текстового редактора.
// file_name: имя (путь до) файла
void open_file_in_editor(WINDOW *editor_win, char *file_name);


// @del_texteditor_window: удаление (закрытие) окна текстового редактора и
// очищение окна родителя;
// texteditor_window: указатель на окно текстового редактора;
// parent: указатель на родительское окно.
void del_texteditor_window(WINDOW *texteditor_window, WINDOW *parent);


// @open_user_file: открытие окна запроса имени файла для открытия. Возвращает
// дескриптор открываемого файла. Используется как вспомогательная функция
// в функции find_and_open_file_in_editor;
// parent: указатель на окно текстового редактора. По нему окно запроса
// определяет свои размеры.
int open_user_file(WINDOW *parent);
