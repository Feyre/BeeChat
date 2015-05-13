/* GUI.cpp
 * ENB241 Project
 *
 * GUI class implementation
 *
 */

#include <cstdlib>
#include <iostream>
#include <vector>
// #include <string> 
#include <ctime>
#include <sstream>
#include <queue>
#include <cmath>
using namespace std; //no need to append std::blah anymore (never put in header)
#include "GUI.h"




string USER = "Feyre   ";
char *Servers_master = "MainChannel:";
char *Servers_sub[] = {"Spawn Area", "Lachlans room", "All talk", "Sports and stuff", "Assignment Help"};
char *graphics[] = {" _____       __      ", "|_   _|     / _|     ", "  | | _ __ | |_ ___  ", "  | || '_ \\|  _/ _ \\ ", " _| || | | | || (_) |", " \\___/_| |_|_| \\___/ "};               
// char *graphics[] = {" _   _            ", "| | | |           ", "| |_| | ___ _   _ ", "|  _  |/ _ \\ | | |", "| | | |  __/ |_| |", "\\_| |_/\\___|\\__, |", "             __/ |", "            |___/ "};

#define NEWLINE_OFFSET 26
#define CHAT_LOG_SIZE 4
#define SUB_SIZE 5
#define SUB_OFFSET 5 //distance away from side
#define CHAT_SIZE 0.8
#define INFO 0
#define SERV 1
#define CHAT 2

time_t timer;
vector<string> DATA;			// "[time] : user : message"
queue<string> MESSAGE_QUEUE;

//store chatlog as single string
//store que of messages
//command block checks for new messages
//command block gives me a message
//swap channel list to user list






GUI::GUI() {

}

GUI::~GUI() {

}

void GUI::initScreen() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	//STORE USERNAME
}

void GUI::initWindows() {
	int row, col;
	getmaxyx(stdscr,row,col);
	info_scr 	= newwin(row, col, 0, 0);
	servers_scr = newwin(row, col, 0, 0);
	chat_win 	= newwin((int)(row*CHAT_SIZE), 	 		(int)(col*CHAT_SIZE)-1, 	0, 						0);
	message_win = newwin((int)(row*(1-CHAT_SIZE))+1, 	col, 						(int)(row*CHAT_SIZE), 	0);
	servers_win = newwin((int)(row*CHAT_SIZE), 			(int)(col*(1-CHAT_SIZE))+1,	0, 						(int)(col*CHAT_SIZE));
    keypad(info_scr, 	TRUE);
    keypad(servers_scr, TRUE);
    keypad(chat_win, 	TRUE);
    keypad(message_win, TRUE);
    keypad(servers_win, TRUE);
    wborder(info_scr, 		'|', '|', '-','-','+','+','+','+');
    wborder(servers_scr, 	'|', '|', '-','-','+','+','+','+');
    wborder(chat_win, 		'|', '|', '-','-','+','+','+','+');
    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
    wborder(servers_win, 	'|', '|', '-','-','+','+','+','+');

	DATA.reserve(100);
	DATA.push_back("[05:30:12] : Feyre    : Hey everyone!");    
	DATA.push_back("[06:25:15] : Mike     : Oh hey :)");    
	DATA.push_back("[05:30:52] : Tim      : Poop");    
	DATA.push_back("[05:30:19] : Feyre    : Uhh, what?");    
}

void GUI::printAscii(WINDOW *scr, char *graphics[], int sizey, int sizex, int starty, int startx) {
	int ii;
	for(ii = 0; ii < sizey; ii++) {
		mvwprintw(scr, starty+ii, startx, graphics[ii]);
	}
}

void GUI::printServers(WINDOW *scr, int a) {
	int ii;
	mvwprintw(scr,1,1, Servers_master);
	for(ii = 0; ii < SUB_SIZE; ii++) {
		if(ii == a) {
			wattron(scr, A_REVERSE);
			mvwprintw(scr,ii+2,SUB_OFFSET, Servers_sub[ii]);
			wattroff(scr, A_REVERSE);
		}
		else {
			mvwprintw(scr,ii+2,SUB_OFFSET, Servers_sub[ii]);
		}
	}
	wrefresh(scr);
}

void GUI::printChat() {
	int row, col;
	getmaxyx(chat_win, row, col);
	wclear(chat_win);
    wborder(chat_win, 	'|', '|', '-','-','+','+','+','+');

    //loop through printing each line
    //if line to long, print what you can
    //then print the last bit on the next line
    //repeat till printed

    //if there is a spaced line, you need to start one chat later.
    //calc how much later first.

    //calc how many lines for this page
	int jj = 1;
	int ii = DATA.size()-row+2;
	if (ii < 0) {ii = 0;}

    int lines = 0;
    int message_len;
    string message;
	for(ii; ii < DATA.size(); ii++) {
		message = DATA[ii];
		message_len = DATA[ii].size();
		if(message_len > col-2) {
			lines++;
			message_len -= (col-4);
			while(message_len > col-2-NEWLINE_OFFSET) {
				lines++;
				message_len -= (col-4-NEWLINE_OFFSET);
			}
			lines++;
		}
		else {
			lines++;
		}
	}
	//test script: im so lonely i only ever listen to myself blah blah blah, i wish i had more friends and stuff im so over everything

	//incriment limit by lines-row+2 (should be zero unless extra line)

	//make so it can read the last few lines without the first/ doesnt matter if you make it scrolling
	int lim = DATA.size()-row+2;	
	if (lim < 0) {lim = 0;}
	lim += lines-row+2;
	if (lim < 0) {lim = 0;}
	// mvwprintw(chat_win,0,0,"%d",lines);
	// mvwprintw(chat_win,0,6," %d ",ii);
	ii = DATA.size()-row+2;
	if (ii < 0) {ii = 0;}

	// ii = message index, starting at n.offset.mess.fit
	// jj = height index,	1, to whatever -- also used to show lines printed
	// lim = n.offset.lines.fit
	for(ii = lim; ii < DATA.size(); ii++) {
		message = DATA[ii];
		message_len = DATA[ii].size();

		if(message_len > col-2) {
			mvwprintw(chat_win,jj,1,(message.substr(0,col-4)+"..").c_str());
			message = message.substr(col-4);
			message_len -= (col-4);
			jj++;
			while(message_len > col-2-NEWLINE_OFFSET) {
				mvwprintw(chat_win,jj,NEWLINE_OFFSET+1,(message.substr(0, col-4-NEWLINE_OFFSET)+"..").c_str());
				message = message.substr(col-4-NEWLINE_OFFSET);
				message_len -= (col-4-NEWLINE_OFFSET);
				jj++;
			}
			mvwprintw(chat_win,jj,NEWLINE_OFFSET+1,message.c_str());
			jj++;
		}
		else {
			mvwprintw(chat_win,jj,1,message.c_str());
			jj++;
		}
	}
	wrefresh(chat_win);
}
//BUGS+FIXES
// Text can print below screen if wrapped (Calc n. of lines)
// Text can print below screen at start if wrapped (add lines and lim seperatly)
// Wrapped text dissapears all at once ()

string GUI::mRequested() {	//check colour 
	string message = MESSAGE_QUEUE.front();
	MESSAGE_QUEUE.pop();
	return message;
}

void GUI::mRecieved(string message) {
	DATA.push_back(message);
	printChat();
}

void GUI::showScreen(int a, int b) {
	switch(a) {
		case INFO:
			int row, col;
			getmaxyx(info_scr,row,col);
			printAscii(info_scr, graphics, 6, 21, (int)(row*0.1), (col-21)/2);
			wrefresh(info_scr);
			break;

		case SERV:
			printServers(servers_scr, b);
			break;

		case CHAT:
			printServers(servers_win, b);
			wrefresh(message_win);
			printChat();
			break;
	}
}

int GUI::selectServer() {
	showScreen(SERV, 0);
	int c = 'o';
	int selection = 0, choice = 0, ii;
	while(1) {	
		c = wgetch(servers_scr);
		switch(c) {	
			case KEY_UP:
				if(selection == 0) {selection = SUB_SIZE-1;}
				else {--selection;}
				break;
			case KEY_DOWN:
				if(selection == SUB_SIZE-1) {selection = 0;}
				else  {++selection;}
				break;
			case 10:
				choice = selection;
				return choice;
				break;
			// default:
			// 	mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
			// 	refresh();
			// 	break;
		}
		showScreen(SERV, selection);
	}	
}

void GUI::userInput() {
	echo();
	curs_set(1);
	char mess_char[80];
	char time_char[11];
	int row, col;
	getmaxyx(message_win, row, col);
	wmove(message_win, row/2, (int)(col*0.1));


	wgetstr(message_win, mess_char);

	//if not null
	string MESSAGE(mess_char);

	time(&timer);
	struct tm * timeinfo;
	timeinfo = localtime(&timer);
	strftime(time_char,11,"[%H:%M:%S]",timeinfo);
	string TIME(time_char);

	string DATA_str = TIME + " : " + USER + " : " + MESSAGE;

	wclear(message_win);	//MAYBE: write own clear function that wont clear border.
    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
	wrefresh(message_win);
	curs_set(0);
	noecho();

	MESSAGE_QUEUE.push(DATA_str);
}





void GUI::Routine() {
	showScreen(INFO, 1);
	wgetch(info_scr);
	int selectedServer = selectServer();
	showScreen(CHAT, selectedServer);

	while(1) {
		userInput();
		string MESSAGE = mRequested();
		mRecieved(MESSAGE);
	}



	//MAIN:
	//loop for user input
	//if found send to queue
	//await recieved message



}

	//BUGS:
	//cpy+pste 2ce kills (segfault)







	//find max username size
	//have option for seconds/noseconds


	// chat_win 	= newwin(/*(int)(row*CHAT_SIZE)*/			row-5, 	 		(int)(col*CHAT_SIZE)-1, 	0, 								0);
	// message_win = newwin(/*(int)(row*(1-CHAT_SIZE))+1*/		row-(row-5), 	col, 						/*(int)(row*CHAT_SIZE)*/row-5, 	0);
	// servers_win = newwin(/*(int)(row*CHAT_SIZE)*/			row-5, 			(int)(col*(1-CHAT_SIZE))+1,	0, 								(int)(col*CHAT_SIZE));

