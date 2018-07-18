// Чат на разделяемой памяти

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>


// Длина имени и длина сообщения
#define NAME_LENGTH 64
#define MESS_LENGTH 1024


// Номер сообщения
int PREV_MESS = -1;


// Структура, описывающая сообщение
struct message {
    int mess_num;
    char name[NAME_LENGTH];
    char message[MESS_LENGTH];
};


// @shm_get_ptr: функция получения указателя по идентификатору;
// shm_id: идентификатор памяти.
void *shm_get_ptr(int shm_id)
{
    void *shm_ptr = shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat()");
        exit(EXIT_FAILURE);
    }
    return shm_ptr;
}


// @shm_connect: Функция получения указателя на "общее" сообщение по ключу;
// key: ключ.
struct message *shm_connect(key_t key)
{
    void *res_ptr;
    int shm_id = shmget(key,
                        sizeof(struct message),
                        IPC_CREAT | IPC_EXCL | 0666);
    if (shm_id < 0 && errno != EEXIST) {
        perror("Key is not created");
        exit(EXIT_FAILURE);
    }
    if (errno == EEXIST) {
        shm_id = shmget(key, sizeof(struct message), 0);
        if (shm_id < 0) {
            perror("Key is not connected");
            exit(EXIT_FAILURE);
        }
        res_ptr = shm_get_ptr(shm_id);
    } else {
        res_ptr = shm_get_ptr(shm_id);
        struct message *fst_mess_ptr = (struct message *)res_ptr;
        strcpy(fst_mess_ptr->name, "Hey!");
        strcpy(fst_mess_ptr->message, "Chat started!");
        fst_mess_ptr->mess_num = 0;
    }
    return (struct message *)res_ptr;
}


// @sem_connect: Функция получения идентификатора семафоров по ключу;
// key: ключ;
// num: число семафоров.
int sem_connect(key_t key, int num)
{
    int sem_id = semget(key,
                        num,
                        IPC_CREAT | IPC_EXCL | 0666);
    if (sem_id < 0 && errno != EEXIST) {
        perror("Semaphore is not created");
        exit(EXIT_FAILURE);
    }
    if (errno == EEXIST) {
        sem_id = semget(key, num, 0);
        if (sem_id < 0) {
            perror("Semaphore is not connected");
            exit(EXIT_FAILURE);
        }
    }
    return sem_id;
}


// @chat_sem_lock: Функция увеличения счетчика семафора sem_id на 1, если
// семафор равен 0;
// sem_id: идентификатор семафора.
void chat_sem_lock(int sem_id)
{
    struct sembuf lock[2] = {
        {0, 0, 0},
        {0, 1, 0}
    };
    if (semop(sem_id, lock, 2) == -1) {
        perror("lock error");
        exit(EXIT_FAILURE);
    }
}


// @chat_sem_unlock: Функция уменьшения счетчика семафора sem_id на 1;
// sem_id: идентификатор семафора.
void chat_sem_unlock(int sem_id)
{
    struct sembuf unlock[1] = {{0, -1, 0}};
    if (semop(sem_id, unlock, 1) == -1) {
        printf("Add nickname in arguments\n");
        exit(EXIT_FAILURE);
    }
}


// @thread_rcv_func: потоковая функция приема сообщений;
// args: аргументы потоковой функции (передается указатель на структуру
// сообщения в разделяемой памяти).
void *thread_rcv_func(void *args)
{
    struct message *shared_message = (struct message *) args;
    while (1) {
        if (shared_message->mess_num != PREV_MESS) {
            PREV_MESS = shared_message->mess_num;
            printf("[%s]%s\n", shared_message->name, shared_message->message);
        }
    }
}


// @send_mess: Отправка сообщения;
// chat_ptr: указатель на структуру сообщения в разделяемой памяти;
// name: имя клиента;
// mess: сообщение.
void send_mess(struct message *chat_ptr, const char *name, const char *mess)
{
    strncpy(chat_ptr->name, name, NAME_LENGTH);
    strncpy(chat_ptr->message, mess, MESS_LENGTH);
    (chat_ptr->mess_num)++;
}


int main(int argc, char const *argv[])
{
    if (argc < 2) {
        perror("Too low arguments");
        exit(EXIT_FAILURE);
    }
    key_t key_shm = ftok("./testmain.buf", 'A');
    key_t key_sem = ftok("./testmain.buf", 'B');
    struct message *chat_ptr = shm_connect(key_shm);
    int sem_id = sem_connect(key_sem, 1);

    pthread_t thread_id;
    int error_code;
    error_code = pthread_create(&thread_id,
                                NULL,
                                thread_rcv_func,
                                (void *)chat_ptr);
    if (error_code != 0) {
        errno = error_code;
        perror("pthread_create error");
        exit(EXIT_FAILURE);
    }
    char message[MESS_LENGTH];
    while(1) {
        scanf("%s", message);
        if (strcmp(message, "exit();") == 0)
            break;
        chat_sem_lock(sem_id);
        send_mess(chat_ptr, argv[1], message);
        chat_sem_unlock(sem_id);
    }
    shmdt(chat_ptr);
    return 0;
}
