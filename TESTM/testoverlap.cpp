/* shapetest.cpp
 * ENB241 Portfolio 2
 *
 * Shape inheritance demonstration
 *
 */

#include "windows.h"
#include <stdlib.h>
#include <iostream>
using namespace std;




int main(int argc, char *argv[])
{
    int ch = 'a';
    Windows mainWindow;
    mainWindow.initWindow();

    // WINDOW *my_win = mainWindow.win_0;
    keypad(mainWindow.win_0, TRUE);
    mainWindow.win_0 = newwin(10, 20, 3, 5);
    mainWindow.win_1 = newwin(10, 20, 8, 15);
    wborder(mainWindow.win_0, '|', '|', '-','-','+','+','+','+');
    wrefresh(mainWindow.win_0);

    sleep(1);

    mvwprintw(mainWindow.win_0, 1, 1, "hey");
    wrefresh(mainWindow.win_0);

    sleep(1);

    wborder(mainWindow.win_1, '|', '|', '-','-','+','+','+','+');
    wrefresh(mainWindow.win_1);

    sleep(1);

    mvwprintw(mainWindow.win_0, 8, 5, "heyyyyyyyy");
    wrefresh(mainWindow.win_0);

    sleep(1);

    // wborder(mainWindow.win_1, '|', '|', '-','-','+','+','+','+');
    wrefresh(mainWindow.win_1);


    // mvwprintw(stdscr, 1, 1, "hey std");
    // wrefresh(stdscr);
    // wrefresh(mainWindow.win_0);


    wgetch(mainWindow.win_0);
    endwin();
    return 0;
}
