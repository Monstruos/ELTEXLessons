#include "file_manager.h"

int main(int argc, char const *argv[])
{
    WINDOW *par_fm_win_1, *par_fm_win_2;
    struct FILE_MANAGER *man_1, *man_2;
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, true);
    par_fm_win_1 = newwin(LINES, COLS / 2, 0, 0);
    par_fm_win_2 = newwin(LINES, COLS / 2, 0, COLS / 2);
    refresh();
    char cur_path[PATH_MAX];
    getcwd(cur_path, PATH_MAX);
    man_1 = init_file_manager_full_window(par_fm_win_1);
    man_2 = init_file_manager_full_window(par_fm_win_2);
    open_dir(man_1, cur_path);
    open_dir(man_2, cur_path);
    mousemask(0, NULL);
    show_cur_dir_with_chosen_file(man_1);
    show_cur_dir_with_chosen_file(man_2);
    int exit_exec_code = -1;
    while(1) {
        exit_exec_code = fm_exec(man_1);
        if (exit_exec_code == CLOSE_CODE)
            break;
        exit_exec_code = fm_exec(man_2);
        if (exit_exec_code == CLOSE_CODE)
            break;
    }
    del_file_manager_window(man_1);
    del_file_manager_window(man_2);
    endwin();
    return 0;
}
