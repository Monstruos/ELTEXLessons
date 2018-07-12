#include "load_win.h"


void init_load_window(struct load_win_info *info)
{
    pthread_create(&(info->load_win_thread), NULL, load_thread_func, info);
}


void load_window_sync(struct load_win_info *info)
{
    pthread_join(info->load_win_thread, NULL);
}


void *load_thread_func(void *args)
{
    struct load_win_info *l_info = args;
    int par_size_x, par_size_y;
    getmaxyx(l_info->parent_win, par_size_y, par_size_x);
    int load_size_x = par_size_x, load_size_y = 5;
    WINDOW *load_win = derwin(l_info->parent_win, load_size_y, load_size_x, 0, 0);
    box(load_win, '|', '-');
    wmove(load_win, 1, 1);
    wprintw(load_win, "Copying...");
    float status;

    while (*(l_info->status) < l_info->size) {
        status = (float) (*(l_info->status)) / (l_info->size);
        int percent = (int)(status * 100);
        int status_bar = status * (load_size_x - 9);
        wmove(load_win, 3, 1);
        wprintw(load_win, "[");
        for (int i = 0; i < status_bar; i++) {
            wprintw(load_win, "#");
        }
        for (int i = status_bar; i < load_size_x - 9; i++) {
            wprintw(load_win, ".");
        }
        wprintw(load_win, "]");
        wprintw(load_win, "[%2i\%]", percent);
        wrefresh(load_win);
    }
    delwin(load_win);
    wclear(l_info->parent_win);
    wrefresh(l_info->parent_win);
}


void copy_file_with_status(WINDOW *init_window,
                           char *orig_file_path,
                           char *copy_file_path)
{
    char s[BLOCK_SIZE];
    int read_bytes = 0;
    int buf_size;
    int file_d_rd = open(orig_file_path, O_RDWR);
    int file_d_wr = creat(copy_file_path, 0666);
    struct stat f_stat;

    stat(orig_file_path, &f_stat);
    int size = f_stat.st_size;
    struct load_win_info load_inf;
    load_inf.size = size;
    load_inf.status = &read_bytes;
    load_inf.parent_win = init_window;
    init_load_window(&load_inf);
    while (buf_size = read(file_d_rd, &s, BLOCK_SIZE)) {
        read_bytes += buf_size;
        write(file_d_wr, &s, buf_size);
    }
    load_window_sync(&load_inf);
    close(file_d_rd);
    close(file_d_wr);
}


char *open_request_copy_file(WINDOW *init_window)
{
    int editor_size_x, editor_size_y;
    getmaxyx(init_window, editor_size_y, editor_size_x);
    WINDOW *request_win = derwin(init_window, 5, editor_size_x - 2, 1, 1);
    box(request_win, '|', '-');
    WINDOW *request_subwin = derwin(request_win, 3, editor_size_x - 4, 1, 1);
    wclear(request_subwin);
    wattron(request_subwin, A_BOLD);
    wprintw(request_subwin, "Enter path to copy file:\n");
    wrefresh(request_subwin);
    wrefresh(request_win);
    char *path_to_file = calloc(PATH_MAX, sizeof(char));
    wmove(request_subwin, 2, 0);
    echo();
    wrefresh(request_subwin);
    wgetnstr(request_subwin, path_to_file, PATH_MAX);
    noecho();
    wclear(init_window);
    delwin(request_subwin);
    delwin(request_win);
    wrefresh(init_window);
    return path_to_file;
}
