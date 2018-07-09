#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <limits.h>

#define MAX_SIZE_OF_TEXT 65536
#define REQUEST_WINDOW_SIZE_X 75
#define REQUEST_WINDOW_SIZE_Y 5


WINDOW *init_texteditor_full_window(WINDOW *parent);
void run_texteditor(WINDOW *editor_win);
void find_and_open_file_in_editor(WINDOW *editor_win);
void open_file_in_editor(WINDOW *editor_win, char *file_path);
void open_and_save_file_in_editor(WINDOW *editor_win);
void del_texteditor_window(WINDOW *texteditor_window, WINDOW *parent);

int open_user_file(WINDOW *parent);
