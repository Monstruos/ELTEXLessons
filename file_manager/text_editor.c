#include "text_editor.h"


WINDOW *init_texteditor_full_window(WINDOW *parent)
{
    int par_size_x, par_size_y;
    getmaxyx(parent, par_size_y, par_size_x);
    box(parent, '|', '-');
    mvwprintw(parent, 0, 2, "[F3]:exit | [F5]:load | [F9]:save");
    WINDOW *return_win = derwin(parent,
                                par_size_y - 2, par_size_x - 2,
                                1, 1);
    wrefresh(parent);
    wrefresh(return_win);
    mousemask(0, NULL);
    return return_win;
}


void run_texteditor(WINDOW *editor_win)
{
    bool exec = false;
    while (!exec) {
        int ch = getch();
        int cur_x, cur_y;
        getyx(editor_win, cur_y, cur_x);
        switch (ch) {
        case KEY_F(5):
            find_and_open_file_in_editor(editor_win);
            break;
        case KEY_F(9):
            open_and_save_file_in_editor(editor_win);
            break;
        case KEY_F(3):
            exec = true;
            break;
        case KEY_UP:
            --cur_y;
            wmove(editor_win, cur_y, cur_x);
            break;
        case KEY_DOWN:
            ++cur_y;
            wmove(editor_win, cur_y, cur_x);
            break;
        case KEY_LEFT:
            --cur_x;
            wmove(editor_win, cur_y, cur_x);
            break;
        case KEY_RIGHT:
            ++cur_x;
            wmove(editor_win, cur_y, cur_x);
            break;
        default:
            waddch(editor_win, ch);
            break;
        }
        wrefresh(editor_win);
    }
}


void find_and_open_file_in_editor(WINDOW *editor_win)
{
    char out_buffer[MAX_SIZE_OF_TEXT] = "";
    int f_descr;
    f_descr = open_user_file(editor_win);
    if(f_descr != -1) {
        read(f_descr, out_buffer, MAX_SIZE_OF_TEXT);
        wprintw(editor_win, out_buffer);
    }
    close(f_descr);
    wmove(editor_win, 0, 0);
    refresh();
    wrefresh(editor_win);
}


void open_file_in_editor(WINDOW *editor_win, char *file_path)
{
    wclear(editor_win);
    char out_buffer[MAX_SIZE_OF_TEXT] = "";
    int f_descr = 0;
    if((f_descr = open(file_path, O_RDWR)) != -1) {
        lseek(f_descr, 0, SEEK_SET);
        read(f_descr, out_buffer, MAX_SIZE_OF_TEXT);
        wprintw(editor_win, out_buffer);
    }
    wrefresh(editor_win);
}


void open_and_save_file_in_editor(WINDOW *editor_win)
{
    char file[MAX_SIZE_OF_TEXT] = "";
    wmove(editor_win, 0, 0);
    winstr(editor_win, file);
    int editor_size_x, editor_size_y;
    getmaxyx(editor_win, editor_size_y, editor_size_x);
    WINDOW *request_win = derwin(editor_win, 5, editor_size_x - 2, 1, 1);
    box(request_win, '|', '-');
    WINDOW *request_subwin = derwin(request_win, 3, editor_size_x - 4, 1, 1);
    wclear(request_subwin);
    wprintw(request_subwin, "Enter path to file:\n");
    wrefresh(request_subwin);
    wrefresh(request_win);
    char path_to_file[PATH_MAX];
    wmove(request_subwin, 2, 0);
    echo();
    wrefresh(request_subwin);
    wgetnstr(request_subwin, path_to_file, PATH_MAX);
    noecho();
    int save_descr = 0;
    if((save_descr = open(path_to_file, O_RDWR)) != -1) {
        wclear(request_subwin);
        wmove(request_subwin, 0, 0);
        wprintw(request_subwin, "Are you sure to rewrite file? (y|n)");
        wrefresh(request_subwin);
        bool answer = false;
        while (!answer) {
            int ch = getch();
            switch (ch) {
            case 'y':
            case 'Y':
                answer = true;
                lseek(save_descr, 0, SEEK_SET);
                write(save_descr, file, MAX_SIZE_OF_TEXT);
                break;
            case 'n':
            case 'N':
                answer = true;
                break;
            }
        }
        close(save_descr);
    } else {
        if ((save_descr = creat(path_to_file, 0666)) != -1) {
            lseek(save_descr, 0, SEEK_SET);
            write(save_descr, file, strlen(file));
            close(save_descr);
        } else {
            wclear(request_subwin);
            wprintw(request_subwin, "Can't create file.");
            wrefresh(request_subwin);
            usleep(3000);
        }
    }
    wclear(editor_win);
    delwin(request_subwin);
    delwin(request_win);
    wprintw(editor_win, file);
    wrefresh(editor_win);
    refresh();
}


void del_texteditor_window(WINDOW *texteditor_window, WINDOW *parent)
{
    delwin(texteditor_window);
    wclear(parent);
    wrefresh(parent);
}


int open_user_file(WINDOW *parent)
{
    int editor_size_x, editor_size_y;
    getmaxyx(parent, editor_size_y, editor_size_x);
    WINDOW *request_win = derwin(parent, 5, editor_size_x - 2, 1, 1);
    box(request_win, '|', '-');
    WINDOW *request_subwin = derwin(request_win, 3, editor_size_x - 4, 1, 1);
    wattron(request_subwin, A_BOLD);
    wprintw(request_subwin, "Enter path to file:\n");
    wrefresh(request_subwin);
    wrefresh(request_win);
    char path_to_file[PATH_MAX];
    wmove(request_subwin, 2, 0);
    echo();
    wrefresh(request_subwin);
    wgetnstr(request_subwin, path_to_file, PATH_MAX);
    noecho();
    int output_descr = 0;
    if((output_descr = open(path_to_file, O_RDWR)) != -1) {
        lseek(output_descr, 0, SEEK_SET);
    } else {
        wclear(request_subwin);
        wprintw(request_subwin, "Can't find file");
        wrefresh(request_subwin);
        usleep(3000);
    }
    wclear(parent);
    delwin(request_subwin);
    delwin(request_win);
    wrefresh(parent);
    return output_descr;
}
