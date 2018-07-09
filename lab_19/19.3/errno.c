#include "errno.h"

pthread_key_t key;
pthread_once_t once;


void init_key()
{
    pthread_key_create(&key, key_destructor);
}


void key_destructor(void *value)
{
    free(value);
}


char *errno(int error_code)
{
    char *ret_str;
    pthread_once(&once, &init_key);
    ret_str = pthread_getspecific(key);
    if (ret_str == NULL) {
        ret_str = malloc(ERROR_STR_LENGTH);
        pthread_setspecific(key, ret_str);
    }
    switch (error_code) {
        case 0:
            strncpy(ret_str, "Code 0", ERROR_STR_LENGTH);
            break;
        case 1:
            strncpy(ret_str, "Code 1", ERROR_STR_LENGTH);
            break;
        case 2:
            strncpy(ret_str, "Code 2", ERROR_STR_LENGTH);
            break;
        case 3:
            strncpy(ret_str, "Code 3", ERROR_STR_LENGTH);
            break;
        case 4:
            strncpy(ret_str, "Code 4", ERROR_STR_LENGTH);
            break;
        default:
            strncpy(ret_str, "", ERROR_STR_LENGTH);
            break;
    }

    return ret_str;
}
