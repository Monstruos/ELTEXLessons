// compile with -lerl_interface -lei -pthread
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "erl_interface.h"
#include "ei.h"

#define BUF_SIZE 1024

int fact(int inp)
{
    return inp > 0 ? inp * fact(inp - 1) : 1;
}

int main(int argc, char const *argv[])
{
    ErlMessage emsg;
    ETERM *fromp, *argp, *resp;
    unsigned char buffer[BUF_SIZE];
    erl_init(NULL, 0);
    erl_connect_init(1, "my_cookie", 0);
    int fd = erl_connect("node1@monstruosPC");
    int loop = 1;
    while (loop) {
        int got = erl_receive_msg(fd, buffer, BUF_SIZE, &emsg);
        switch (got) {
        case ERL_TICK:
            break;
        case ERL_ERROR:
            loop = 0;
            break;
        default:
            if (emsg.type == ERL_REG_SEND) {
                fromp = erl_element(1, emsg.msg);
                argp = erl_element(2, emsg.msg);
                resp = erl_format("{ok, ~i}", fact(ERL_INT_VALUE(argp)));
                erl_send(fd, fromp, resp);
                erl_free_term(emsg.from);
                erl_free_term(emsg.msg);
                erl_free_term(fromp);
                erl_free_term(argp);
                erl_free_term(resp);
            }
            break;
        }
    }
    return 0;
}
