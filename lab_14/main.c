#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "calc.h"


void clear_stdin_buffer()
{
    char input;
    while((input = fgetc(stdin)) != EOF && input != '\n');
}


int main(int argc, char const *argv[])
{
    int first_number, second_number;
    float result;
    int menu_selector = -1;

    while(1) {
        printf("1) add(a, b);\n");
        printf("2) sub(a, b);\n");
        printf("3) multi(a, b);\n");
        printf("4) division(a, b);\n");
        printf("5) Quit;\n");
        scanf("%i", &menu_selector);
        clear_stdin_buffer();
        if(menu_selector == 5) {
            printf("exit...\n");
            return 0;
        }
        printf("a?");
        scanf("%i", &first_number);
        clear_stdin_buffer();
        printf("b?");
        scanf("%i", &second_number);
        clear_stdin_buffer();
        switch(menu_selector) {
            case 1: {
                result = add(first_number, second_number);
                break;
            }
            case 2: {
                result = sub(first_number, second_number);
                break;
            }
            case 3: {
                result = multi(first_number, second_number);
                break;
            }
            case 4: {
                result = division(first_number, second_number);
                break;
            }
            default: {
                printf("Wrong action\n");
                break;
            }
        }
        system("clear");
        printf(">%f\n", result);
    }
    return 0;
}
