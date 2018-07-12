#include "file_manager.h"

struct FILE_MANAGER *init_file_manager_full_window(WINDOW *parent)
{
    int par_size_x, par_size_y;
    getmaxyx(parent, par_size_y, par_size_x);
    box(parent, '|', '-');
    mvwprintw(parent, 0, 2, "FILE_MANAGER");
    WINDOW *fm_win = derwin(parent,
                            par_size_y - 2, par_size_x - 2,
                            1, 1);
    struct FILE_MANAGER *return_fm =
    malloc(sizeof(struct FILE_MANAGER));
    return_fm->fm_window = fm_win;
    return_fm->parent = parent;
    strncpy(return_fm->dir_path, "/", PATH_MAX);
    return_fm->file_list = NULL;
    return_fm->file_list_ptr = 0;
    open_dir(return_fm, "/");
    wrefresh(return_fm->parent);
    wrefresh(return_fm->fm_window);
    return return_fm;
}


int show_cur_dir_with_chosen_file(struct FILE_MANAGER *fm)
{
    wclear(fm->fm_window);
    for(int it = 0; it < fm->file_count; ++it) {
        if (it == fm->file_list_ptr)
            wattron(fm->fm_window, A_STANDOUT);
        wprintw(fm->fm_window, "%s\n", fm->file_list[it]->d_name);
        wattroff(fm->fm_window, A_STANDOUT);
    }
    wrefresh(fm->fm_window);
    return 0;
}


int chosen_is_dir(struct FILE_MANAGER *fm)
{
    char *file_path = get_chosen_file_path(fm);
    if (!is_directory(file_path)) {
        free(file_path);
        return 0;
    }
    free(file_path);
    return 1;
}


int chosen_is_exec(struct FILE_MANAGER *fm)
{
    char *file_path = get_chosen_file_path(fm);
    if (!is_exec(file_path)) {
        free(file_path);
        return 0;
    }
    free(file_path);
    return 1;
}


int open_chosen_dir(struct FILE_MANAGER *fm)
{
    char *file_path = get_chosen_file_path(fm);
    char resolved_file_path[PATH_MAX];
    realpath(file_path, resolved_file_path);
    free(file_path);
    return open_dir(fm, resolved_file_path);
}


void del_file_manager_window(struct FILE_MANAGER *fm)
{
    close_curr_dir(fm);
    delwin(fm->fm_window);
    wclear(fm->parent);
    wrefresh(fm->parent);
    free(fm);
}


int set_file_ptr(struct FILE_MANAGER *fm, int file_ptr)
{
    if (file_ptr < 0 || file_ptr >= fm->file_count)
        return 1;
    fm->file_list_ptr = file_ptr;
    return 0;
}


char *get_chosen_file_path(struct FILE_MANAGER *fm)
{
    char *file_path = malloc(sizeof(char) * PATH_MAX);
    strncpy(file_path, fm->dir_path, PATH_MAX);
    strcat(file_path, "/");
    strcat(file_path, fm->file_list[fm->file_list_ptr]->d_name);
    return file_path;
}


int open_dir(struct FILE_MANAGER *fm, const char *dir_path)
{
    strncpy(fm->dir_path, dir_path, PATH_MAX);
    if (fm->file_list != NULL) {
        close_curr_dir(fm);
        fm->file_list = NULL;
    }
    fm->file_count = scandir(fm->dir_path, &(fm->file_list),
                             NULL, alphasort);
    if (fm->file_count < 0)
        return 1;
    fm->file_list_ptr = 0;
    return 0;
}


void close_curr_dir(struct FILE_MANAGER *fm)
{
    while ((fm->file_count)--) {
        free(fm->file_list[fm->file_count]);
    }
    free(fm->file_list);
}


int fm_exec(struct FILE_MANAGER *fm)
{
    int exec = 0;
    int select_pos = fm->file_list_ptr;
    char *orig_file_path, *copy_file_path;
    open_dir(fm, fm->dir_path);
    wattron(fm->parent, A_STANDOUT);
    box(fm->parent, '|', '-');
    mvwprintw(fm->parent, 0, 2, "FILE_MANAGER");
    wrefresh(fm->parent);
    wrefresh(fm->fm_window);
    show_cur_dir_with_chosen_file(fm);
    while (!exec) {
        int ch = getch();
        switch (ch) {
        case KEY_UP:
            select_pos--;
            if (set_file_ptr(fm, select_pos) == 1)
                select_pos++;
            break;
        case KEY_DOWN:
            select_pos++;
            if (set_file_ptr(fm, select_pos) == 1)
                select_pos--;
            break;
        case KEY_RIGHT:
        case '\n':
            if (chosen_is_dir(fm)) {
                open_chosen_dir(fm);
                select_pos = 0;
                set_file_ptr(fm, select_pos);
            } else {
                if (chosen_is_exec(fm)) {
                    run_chosen_exec_file(fm);
                    refresh();
                    wrefresh(fm->parent);
                    wrefresh(fm->fm_window);
                } else {
                    WINDOW *te_win;
                    te_win = init_texteditor_full_window(fm->fm_window);
                    curs_set(TRUE);
                    char *file_path = get_chosen_file_path(fm);
                    open_file_in_editor(te_win, file_path);
                    run_texteditor(te_win);
                    curs_set(FALSE);
                    free(file_path);
                    del_texteditor_window(te_win, fm->fm_window);
                    refresh();
                    wrefresh(fm->parent);
                    wrefresh(fm->fm_window);
                }
            }
            break;
        case 27:
            exec = CLOSE_CODE;
            break;
        case '\t':
            exec = SWITCH_CODE;
            break;
        case 'c':
        case 'C':
            orig_file_path = get_chosen_file_path(fm);
            copy_file_path = open_request_copy_file(fm->fm_window);
            copy_file_with_status(fm->fm_window,
                                  orig_file_path,
                                  copy_file_path);
            free(orig_file_path);
            free(copy_file_path);
            open_dir(fm, fm->dir_path);
        default:
            break;
        }
        show_cur_dir_with_chosen_file(fm);
        refresh();
        wrefresh(fm->parent);
        wrefresh(fm->fm_window);
    }
    wattroff(fm->parent, A_STANDOUT);
    box(fm->parent, '|', '-');
    mvwprintw(fm->parent, 0, 2, "FILE_MANAGER");
    wrefresh(fm->parent);
    wrefresh(fm->fm_window);
    return exec;
}


void run_chosen_exec_file(struct FILE_MANAGER *fm)
{
    char *file_path = get_chosen_file_path(fm);
    pid_t exec_proc;
    endwin();
    exec_proc = fork();
    switch (exec_proc) {
    case 0:
        execl(file_path,
              fm->file_list[fm->file_list_ptr]->d_name,
              (char *) NULL);
        break;
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;
    default:
        wait(NULL);
        sleep(4);
        break;
    }
    free(file_path);
}


int is_directory(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}


int is_exec(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    if (statbuf.st_mode & S_IXUSR)
        return 1;
    return 0;
}
