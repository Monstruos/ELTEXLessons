#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char const *argv[]) {
    int rdwr_f_descr = open(argv[1], O_RDWR);
    struct stat f_stat;
    stat(argv[1], &f_stat);
    int size = f_stat.st_size;
    char out_buffer[BUF_SIZE] = "";
    int offset;
    for (offset = 1; offset <= size; ++offset) {
        lseek(rdwr_f_descr, -offset, SEEK_END);
        read(rdwr_f_descr,  &out_buffer[offset - 1], 1);
    }
    write(STDOUT_FILENO, out_buffer, strlen(out_buffer));
    close(rdwr_f_descr);
    return 0;
}
