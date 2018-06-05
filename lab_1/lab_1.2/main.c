#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define ACTION_LENGTH 10
enum ACTIVE_ACTION
{
    NO_ACTION,
    ADD_ACTION,
    SUB_ACTION,
    MUL_ACTION,
    DIV_ACTION,
    LOG_ACTION
};


extern float add(int a, int b);
extern float sub(int a, int b);
extern float multi(int a, int b);
extern float division(int a, int b);
extern float logarithm(int a);

/*
 * set_active_action
 * @input: string with an action
 * Description: convert action from string to ACTIVE_ACTION
 */
enum ACTIVE_ACTION set_active_action(char *input)
{
    enum ACTIVE_ACTION output_action;
    if (strcmp(input, "+") == 0) {
        output_action = ADD_ACTION;
    } else if (strcmp(input, "-") == 0) {
        output_action = SUB_ACTION;
    } else if (strcmp(input, "*") == 0) {
        output_action = MUL_ACTION;
    } else if (strcmp(input, "/") == 0) {
        output_action = DIV_ACTION;
    } else if (strcmp(input, "log") == 0) {
        output_action = LOG_ACTION;
    } else {
        output_action = NO_ACTION;
    }
    return output_action;
}


int main(int argc, char const *argv[])
{
    int first_number, second_number;
    char *action = (char *) malloc(ACTION_LENGTH);
    enum ACTIVE_ACTION action_switch;
    float result;

    printf("\nCalculator(pre-alpha 0.0000000001b <Now with logarithm!>)\n");
    printf("Print first number (any number, if you want to calc logarithm):\n");
    printf(">");
    scanf("%i\n", &first_number);

    printf("Print action: ( + | - | * | / | log )\n");
    printf(">");
    scanf("%s\n", action);
    action_switch = set_active_action(action);

    printf("Print second number:\n");
    printf(">");
    scanf("%i\n", &second_number);

    switch (action_switch) {
    case ADD_ACTION:
        result = add(first_number, second_number);
    case SUB_ACTION:
        result = sub(first_number, second_number);
    case MUL_ACTION:
        result = multi(first_number, second_number);
    case DIV_ACTION:
        result = division(first_number, second_number);
    case LOG_ACTION:
        result = logarithm(second_number);
    case NO_ACTION:
        printf("Wrong action. Calculator has broken.\n");
        return 1;
    }
    printf(">%f\n", result);
    free(action);
    return 0;
}
