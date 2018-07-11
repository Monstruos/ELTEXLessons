// Данный редактор на правах владельца данного репозитория обьявляю устаревшим,
// все обновления, исправления будут проводиться непосредственно в файловом
// менеджере, в который данный редактор встроен.


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


// Максимальный размер открываемого файла.
#define MAX_SIZE_OF_TEXT 65536
// Размеры окна запроса имени файла
#define REQUEST_WINDOW_SIZE_X 75
#define REQUEST_WINDOW_SIZE_Y 5

// @init_texteditor_full_window: инициализация окна текстового редактора внутри
// родительского окна;
// note: содержимое родительского окна будет перезаписано;
// parent: указатель на родительское окно.
WINDOW *init_texteditor_full_window(WINDOW *parent);


// @run_texteditor: запуск текстового редактора;
// editor_win: указатель на окно текстового редактора.
void run_texteditor(WINDOW *editor_win);


// @find_and_open_file_in_editor: открывает окно запроса имени файла для
// открытия и открывает его в редакторе;
// editor_win: указатель на окно текстового редактора.
void find_and_open_file_in_editor(WINDOW *editor_win);


// @open_and_save_file_in_editor: открывает окно запроса имени файла для
// сохранения. Если файл существует, он будет перезаписан, иначе будет создан
// новый. Если по каким-то причинам создание файла невозможно, будет выведено
// сообщение об ошибке;
// editor_win: указатель на окно текстового редактора.
void open_and_save_file_in_editor(WINDOW *editor_win);

// @del_texteditor_window: удаление (закрытие) окна текстового редактора и
// очищение окна родителя;
// texteditor_window: указатель на окно текстового редактора;
// parent: указатель на родительское окно.
void del_texteditor_window(WINDOW *texteditor_window, WINDOW *parent);


// @open_user_file: открытие окна запроса имени файла для открытия. Возвращает
// дескриптор открываемого файла. Используется как вспомогательная функция
// в функции find_and_open_file_in_editor;
// parent: указатель на окно текстового редактора.
int open_user_file(WINDOW *parent);

int main(int argc, char const *argv[])
{
    WINDOW *txt_editor;
    initscr();
    txt_editor = init_texteditor_full_window(stdscr);
    run_texteditor(txt_editor);
    del_texteditor_window(txt_editor, stdscr);
    endwin();
    return 0;
}

WINDOW *init_texteditor_full_window(WINDOW *parent)
{
    keypad(parent, true);
    noecho();
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


void open_and_save_file_in_editor(WINDOW *editor_win)
{
    char file[MAX_SIZE_OF_TEXT];
    winstr(editor_win, file);
    int editor_size_x, editor_size_y;
    getmaxyx(editor_win, editor_size_y, editor_size_x);
    WINDOW *request_wnd = derwin(editor_win,
                                 REQUEST_WINDOW_SIZE_Y,
                                 REQUEST_WINDOW_SIZE_X,
                                 editor_size_y / 2 -
                                 REQUEST_WINDOW_SIZE_Y / 2,
                                 editor_size_x / 2 -
                                 REQUEST_WINDOW_SIZE_X / 2);
    box(request_wnd, '|', '-');
    WINDOW *request_subwnd = derwin(request_wnd,
                                    REQUEST_WINDOW_SIZE_Y - 2,
                                    REQUEST_WINDOW_SIZE_X - 2,
                                    1, 1);
    wprintw(request_subwnd, "Enter path to file:\n");
    wrefresh(request_subwnd);
    wrefresh(request_wnd);
    char path_to_file[PATH_MAX];
    wmove(request_subwnd, 2, 0);
    echo();
    wrefresh(request_subwnd);
    wgetnstr(request_subwnd, path_to_file, PATH_MAX);
    noecho();
    int save_descr = 0;
    if((save_descr = open(path_to_file, O_RDWR)) != -1) {
        wclear(request_subwnd);
        wmove(request_subwnd, 0, 0);
        wprintw(request_subwnd, "Are you sure to rewrite file? (y|n)");
        wrefresh(request_subwnd);
        bool answer = false;
        while (!answer) {
            int ch = getch();
            switch (ch) {
            case 'y':
            case 'Y':
                answer = true;
                lseek(save_descr, 0, SEEK_SET);
                write(save_descr, file, strlen(file));
                break;
            case 'n':
            case 'N':
                answer = true;
                break;
            }
        }
        close(save_descr);
    } else {
        if ((save_descr = open(path_to_file, O_CREAT|O_WRONLY|O_TRUNC)) != -1) {
            lseek(save_descr, 0, SEEK_SET);
            write(save_descr, file, strlen(file));
            close(save_descr);
        } else {
            wclear(request_subwnd);
            wprintw(request_subwnd, "Can't create file.");
            wrefresh(request_subwnd);
            usleep(3000);
        }
    }
    wclear(editor_win);
    delwin(request_subwnd);
    delwin(request_wnd);
    wprintw(editor_win, file);
    wrefresh(editor_win);
    refresh();
}


void del_texteditor_window(WINDOW *texteditor_window, WINDOW *parent)
{
    delwin(texteditor_window);
    wrefresh(parent);
}


int open_user_file(WINDOW *parent)
{
    int editor_size_x, editor_size_y;
    getmaxyx(parent, editor_size_y, editor_size_x);
    WINDOW *request_wnd = derwin(parent,
                                 REQUEST_WINDOW_SIZE_Y,
                                 REQUEST_WINDOW_SIZE_X,
                                 editor_size_y / 2 -
                                 REQUEST_WINDOW_SIZE_Y / 2,
                                 editor_size_x / 2 -
                                 REQUEST_WINDOW_SIZE_X / 2);
    box(request_wnd, '|', '-');
    WINDOW *request_subwnd = derwin(request_wnd,
                                    REQUEST_WINDOW_SIZE_Y - 2,
                                    REQUEST_WINDOW_SIZE_X - 2,
                                    1, 1);
    wattron(request_subwnd, A_BOLD);
    wprintw(request_subwnd, "Enter path to file:\n");
    wrefresh(request_subwnd);
    wrefresh(request_wnd);
    char path_to_file[PATH_MAX];
    wmove(request_subwnd, 2, 0);
    echo();
    wrefresh(request_subwnd);
    wgetnstr(request_subwnd, path_to_file, PATH_MAX);
    noecho();
    int output_descr = 0;
    if((output_descr = open(path_to_file, O_RDWR)) != -1) {
        lseek(output_descr, 0, SEEK_SET);
    } else {
        wclear(request_subwnd);
        wprintw(request_subwnd, "Can't find file");
        wrefresh(request_subwnd);
        usleep(3000);
    }
    wclear(parent);
    delwin(request_subwnd);
    delwin(request_wnd);
    wrefresh(parent);
    return output_descr;
}
