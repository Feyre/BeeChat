#include <ncurses.h>

int main() {
	int ch = 'a';
	char *str = "hey";

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	WINDOW *my_win;
	keypad(my_win, TRUE);
	my_win = newwin(10, 20, 3, 5);
	wborder(my_win, '|', '|', '-','-','+','+','+','+');
	wrefresh(my_win);

	// ch = getch();
	// wrefresh(my_win);







    while(1);
	return 0;
}
