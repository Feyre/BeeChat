#include <ncurses.h>

int main() {
	int ch = 'a';

	initscr();			/* Start curses mode 		*/
	// raw();				/* Line buffering disabled	*/
	cbreak();
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

    printw("Type any character to see it in bold\n");

    while(1) {
		ch = getch();					/* If raw() hadn't been called
						 				* we have to press enter before it
						 				* gets to the program 		*/
		if(ch == KEY_F(1))					/* Without keypad enabled this will */
			printw("F1 Key pressed");			/*  not get to us either	*/
										/* Without noecho() some ugly escape
						 				* charachters might have been printed
						 				* on screen			*/
		else
		{	printw("The pressed key is ");
			attron(A_STANDOUT);
			printw("%c\n", ch);
			attroff(A_STANDOUT);
		}
		refresh();
	}

	refresh();			/* Print it on to the real screen */
    getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */


	return 0;
}





// INIT: 
// initscr() 		-- starts curser mode.
// refresh() 		-- dump window to screen.
// clear()			-- clears window
// endwin()  		-- end window.

// OUTPUT: 
// move(y,x) 		-- move curser
// addch('a')  		-- add character
// addstr(abc)		-- add string
// addnstr(n,"abc")	-- add at max, n chars.
// printw("abc") 	-- add fromated string
// mvprintw(y,x,"abc") -- move and print string
// EXTRAS: mv, w.

// INPUT:
// getch()			-- get character
// scanw()			-- get formatted input
// getstr()			-- get strings

// ATTRIBUTES:
// A_NORMAL        Normal display (no highlight)
// A_STANDOUT      Best highlighting mode of the terminal.
// A_UNDERLINE     Underlining
// A_REVERSE       Reverse video
// A_BLINK         Blinking
// A_DIM           Half bright
// A_BOLD          Extra bright or bold
// A_PROTECT       Protected mode
// A_INVIS         Invisible or blank mode
// A_ALTCHARSET    Alternate character set
// A_CHARTEXT      Bit-mask to extract a character
// COLOR_PAIR(n)   Color-pair number n 
//
// attron()			-- turns on
// attroff()		-- turns off
// attrset()		-- fully overrides
// attr_get()		-- gets current attributes
// chgat()			-- changes on screen text (n, A_BOLD, 0, NULL) - (n starts from cursor) use -1 for full line
// eg. attron(A_BOLD | A_BLINK)

// COLOURS:
// has_colors() 	-- true if terminal supports colours
// start_color() 	-- starts colour mode
// init_pair(1,foregroud,background)
// attroff(COLOR_PAIR(1))

// WINDOW:
// getmaxyx(win,row,col)
// getyx(win,row,col)

// MISC:
// curs_set(0/1/2) -- can turn curser off/on/supervisible

// LINES/COLS
// COLOUR_RED
// KEY_LEFT
// KEY_F(5)




// windows: where stuff is displayed
// panels:	where windows are attatched
// menus: 	display a line menu
// forms:	area to write responce

// raw/cbreak (stops line buffering) (cbreak allows ctrl+c exit)
// echo/noecho (stops echoing back to terminal)
// keypad(stdscr, TRUE) (enables F1,F2,arrow keys etc..)
// halfdelay (waits X tenths of a second for input then returns ERR) --used then you want to CHECK for input

// A_BOLD, A_UNDERLIEN -- addch(ch | A_BOLD) or attron/attroff(A_BOLD) -- or it with the macro


// call wrefresh, wprintw etc with (win, "hi") when using other windows.

// g++ -ansi -o ncuba ncuba.cpp -lncurses

//SIZES:
// KEY_RESIZE - a ch sent to getch when user resizes screen
// resize_term(int lines, int columns);
// wresize(window, lines, colums)
// mvwin(window, y, x)

