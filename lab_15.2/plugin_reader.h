#ifndef __PLUGIN_READER__H
#define __PLUGIN_READER__H
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fnmatch.h>
#include <string.h>
#include <limits.h>


// note: для упрощения считаем, что все функции имеют вид:
//     float func(int, int)


//MAX_F_NAME_LENGTH: максимальная длина имени файла.
#define MAX_F_NAME_LENGTH 256


// note: каждая библиотека должна содержать информацию о названии функций и их
// количестве. Пример:
//     #define F_NAME_LENGTH 256
//     #define F_COUNT 1
//     const int f_count = F_COUNT;
//     const char f_name[F_COUNT][F_NAME_LENGTH] = {"add"};
// F_NAME_LENGTH = MAX_F_NAME_LENGTH;
// F_COUNT: количество функций в библиотеке;
// f_count: необходим для обнаружения количества функций plugin_reader'ом
// f_name: список имен функций.


// @struct plugin_func_struct: структура, описывающая функции и реализующая
// список функций, считанных из библиотек;
// f_ptr: указатель на функцию;
// f_name: имя функции;
// next: указатель на следующую функцию.
struct plugin_func_struct {
    float (*f_ptr) (int, int);
    char *f_name;
    struct plugin_func_struct *next;
};


// @plugin_func_list_init: функция, производящая поиск библиотек в директории
// dir_name и инициализирующая список функций из библиотек в f_list,
// возвращает 0 в случае успешного считывания всех библиотек;
// dir_name: директория, в которой производится поиск;
// f_list: после отработки функции: указатель на указатель на голову (head)
// списка функций.
// note: меняет рабочую директорию!
int plugin_func_list_init(const char *dir_name,
                          struct plugin_func_struct **f_list);


// @plugin_filter: функция проверки соответствия формата файла, описываемого
// ent формату .so. Используется как вспомогательная функция в
// plugin_func_list_init и не должна использоваться в пользовательском коде,
// возвращает 1 в случае соответствия, иначе 0 (используется как булева
// функция);
// ent: информация о файле.
int plugin_filter(const struct dirent *ent);


// @plugin_func_list_free: функция освобождения памяти, занимаемой списком
// f_list;
// f_list: указатель на список функций.
void plugin_func_list_free(struct plugin_func_struct **f_list);


// @get_plugin_f: функция получения информации о функции по его порядковому
// номеру в списке. Возвращает указатель на структуру, если в списке, начиная
// от f_list, есть функция, иначе NULL;
// note: нумерация производится с 1, то есть, чтобы получить следующую за f_list
// функцию, необходимо вызвать функцию с параметрами:
//     get_plugin_f(f_list, 2);
// f_list: указатель на первую структуру (не на список!);
// number: порядковый номер функции в списке.
struct plugin_func_struct *get_plugin_f(struct plugin_func_struct *f_list,
                                        const unsigned int number);

#endif // __PLUGIN_READER__H
