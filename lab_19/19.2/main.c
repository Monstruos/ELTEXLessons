// Реализация "Магазина"


#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


// Используется для реентерабельного рандомайзера
#define PRNG_BUFSIZE 32
// Количество магазинов
#define STORE_COUNT 5
// Количество потребителей
#define CONS_COUNT 3
// Количество грузчиков
#define LOAD_COUNT 5
// Время сна грузчиков (с)
#define L_SLEEP_TIME 1
// Время сна потребителей (с)
#define C_SLEEP_TIME 2
// Начальные потребности потребителей
#define START_CONSUM_DEMANDS 10000

// Изначальная загрузка магазинов
unsigned int STORE[STORE_COUNT] = {1000, 1000, 1000, 1000, 1000};
// Мьютексы для магазинов
pthread_mutex_t lock[STORE_COUNT];

// Данные переменные используются для нумерации потребителей и грузчиков
int CONS_INDEX = 0;
int LOAD_INDEX = 0;

// Функция работы грузчика
void *loader_func(void *args)
{
    int index = ++LOAD_INDEX;
    int store_num;
    int input_value;
    while (1) {
        random_r((struct random_data *)args, &store_num);
        store_num %= 5;
        printf("Loader[%i] -> (#%i)\n", index, store_num);
        // Попытка зайти в магазин
        if (pthread_mutex_trylock(&lock[store_num]) == 0) {
            random_r((struct random_data *)args, &input_value);
            input_value = (input_value % 100) + 450;
            printf("Loader[%i] (#%i) [%i] --(%i)-> [%i] and sleep on %i s\n",
                   index, store_num, STORE[store_num], input_value,
                   (STORE[store_num] + input_value), L_SLEEP_TIME);
            STORE[store_num] += input_value;
            pthread_mutex_unlock(&lock[store_num]);
        } else {
            printf("Loader[%i] -x-> (#%i), sleep on %i s\n", index, store_num,
                   L_SLEEP_TIME);
        }
        sleep(L_SLEEP_TIME);
    }
    pthread_exit(NULL);
}


// Функция работы потребителей
void *consum_func(void *args)
{
    int index = ++CONS_INDEX;
    int store_num;
    int cons_dem = START_CONSUM_DEMANDS;
    while (1) {
        random_r((struct random_data *)args, &store_num);
        store_num %= 5;
        printf("Consumer[%i] -> (#%i)\n", index, store_num);
        // Попытка зайти в магазин
        if (pthread_mutex_trylock(&lock[store_num]) == 0) {
            int cons_buf = cons_dem - STORE[store_num];
            // Данный потребитель не удовлетворил свои потребности
            if (cons_buf > 0) {
                printf("Consumer[%i] (#%i) [%i] --(%i)-> [%i] (%i -> %i) "
                       "and sleep on %i s\n", index,
                       store_num, cons_dem, STORE[store_num], cons_buf,
                       STORE[store_num], 0, C_SLEEP_TIME);
                STORE[store_num] = 0;
                cons_dem = cons_buf;
            } else {
                printf("Consumer[%i] (#%i) [%i] --(%i)-> [%i] (%i -> %i) "
                       "and stop shopping\n", index,
                       store_num, cons_dem, cons_dem,
                       0, STORE[store_num], -1 * cons_buf);
                STORE[store_num] = -1 * cons_buf;
                break;
            }
            pthread_mutex_unlock(&lock[store_num]);
        } else {
            printf("Consumer[%i] -x-> (#%i): sleep on %i s\n", index,
                   store_num, C_SLEEP_TIME);
        }
        sleep(C_SLEEP_TIME);
    }
    pthread_mutex_unlock(&lock[store_num]);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    // Данные для рандомайзера
    struct random_data *rand_states;
    char *rand_statebufs;
    rand_states = calloc(CONS_COUNT + LOAD_COUNT, sizeof(struct random_data));
    rand_statebufs = calloc(CONS_COUNT + LOAD_COUNT, PRNG_BUFSIZE);

    pthread_t consum[CONS_COUNT];
    pthread_t loader[LOAD_COUNT];

    // Инициализация мьютексов
    for (int it = 0; it < STORE_COUNT; ++it) {
        if (pthread_mutex_init(&lock[it], NULL) != 0) {
            perror("pthread_mutex_init error");
            exit(EXIT_FAILURE);
        }
    }

    // Запуск потребителей
    for (int it = 0; it < CONS_COUNT; ++it) {
        initstate_r(random(), &rand_statebufs[it],
                    PRNG_BUFSIZE, &rand_states[it]);
        int error_code;
        error_code = pthread_create(&consum[it], NULL,
                                    consum_func, &rand_states[it]);
        if (error_code != 0) {
            errno = error_code;
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
    }

    // Запуск грузчиков
    for (int it = CONS_COUNT; it < (CONS_COUNT + LOAD_COUNT); ++it) {
        int ind = it - CONS_COUNT;
        initstate_r(random(), &rand_statebufs[it],
                    PRNG_BUFSIZE, &rand_states[it]);
        int error_code;
        error_code = pthread_create(&loader[ind], NULL,
                                    loader_func, &rand_states[it]);
        if (error_code != 0) {
            errno = error_code;
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int it = 0; it < CONS_COUNT; ++it)
        pthread_join(consum[it], NULL);
    for (int it = 0; it < STORE_COUNT; ++it)
        pthread_mutex_destroy(&lock[it]);
    return 0;
}
