#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "plugin_reader.h"

#define F_NAME_LENGTH 256


void clear_stdin_buffer()
{
    char input;
    while ((input = fgetc(stdin)) != EOF && input != '\n');
}


int main(int argc, char const *argv[])
{
    int first_number, second_number;
    float result;
    int menu_selector = -1;

    char plugin_directory[] = "./plugins/so";

    struct plugin_func_struct *f_list, *f_iter, *chosen_f;
    if (plugin_func_list_init(plugin_directory, &f_list) == 1) {
        perror("plugin_func_list_init error");
        return 1;
    }

    while(1) {
        printf("0) Quit;\n");
        f_iter = f_list;
        for(int it = 1; f_iter != NULL; ++it, f_iter = f_iter->next) {
            printf("%i) %s(a, b)\n", it, f_iter->f_name);
        }
        scanf("%i", &menu_selector);
        clear_stdin_buffer();
        if (menu_selector == 0) {
            printf("exit...\n");
            return 0;
        }
        if((chosen_f = get_plugin_f(f_list, menu_selector)) != NULL) {
            printf("a?");
            scanf("%i", &first_number);
            clear_stdin_buffer();
            printf("b?");
            scanf("%i", &second_number);
            clear_stdin_buffer();
            system("clear");
            printf(">%f\n", chosen_f->f_ptr(first_number, second_number));
        } else {
            system("clear");
            printf(">Wrong action.\n");
        }
    }
    plugin_func_list_free(&f_list);
    return 0;
}
