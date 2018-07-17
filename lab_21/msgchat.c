#include "msgchat.h"


void send_in_queue(int id, struct msgbuf *mess)
{
    if (msgsnd(id,
               (void *) mess,
               sizeof(struct msgbuf) - sizeof(long),
               IPC_NOWAIT) == -1) {
        perror("msgsnd() error");
        exit(EXIT_FAILURE);
    }
}


void receive_from_queue(int id, struct msgbuf *mess, long type)
{
    if (msgrcv(id,
               (void *) mess,
               sizeof(struct msgbuf) - sizeof(long),
               type,
               MSG_NOERROR) == -1) {
        perror("msgrcv() error");
        exit(EXIT_FAILURE);
    }
}
