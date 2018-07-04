#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char const *argv[]) {
    int rdwr_f_descr = open(argv[1], O_RDWR);
    char out_buffer[BUF_SIZE] = "";
    read(rdwr_f_descr, out_buffer, BUF_SIZE);
    int str_size = strlen(out_buffer);
    for (int i = 0; i < (str_size - 1) / 2; i++) {
        out_buffer[i] ^= out_buffer[str_size - 1 - i] ^=
        out_buffer[i] ^= out_buffer[str_size - 1 - i];
    }
    write(STDOUT_FILENO, out_buffer, strlen(out_buffer));
    close(rdwr_f_descr)
    return 0;
}
