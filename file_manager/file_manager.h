#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <limits.h>
#include "text_editor.h"

#define SWITCH_CODE 1
#define CLOSE_CODE 2


struct FILE_MANAGER {
    WINDOW *fm_window;
    WINDOW *parent;
    struct dirent **file_list;
    char dir_path[PATH_MAX];
    int file_count;
    int file_list_ptr;
};


struct FILE_MANAGER *init_file_manager_full_window(WINDOW *parent);
int show_cur_dir_with_chosen_file(struct FILE_MANAGER *fm);
int chosen_is_dir(struct FILE_MANAGER *fm);
int chosen_is_exec(struct FILE_MANAGER *fm);
int open_chosen_dir(struct FILE_MANAGER *fm);
void del_file_manager_window(struct FILE_MANAGER *fm);
int set_file_ptr(struct FILE_MANAGER *fm, int file_ptr);
char *get_chosen_file_path(struct FILE_MANAGER *fm);
int set_fm_dir(struct FILE_MANAGER *fm, char *dir_path);
int open_dir(struct FILE_MANAGER *fm, const char *dir_name);
void close_curr_dir(struct FILE_MANAGER *fm);
int fm_exec(struct FILE_MANAGER *fm);
void run_chosen_exec_file(struct FILE_MANAGER *fm);

int is_directory(const char *path);
int is_exec(const char *path);
