/* GUI.cpp
 * ENB241 Project
 *
 * GUI class implementation
 *
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <cmath>
#include <cctype>
#include "GUI.h"
using namespace std;

// TODO:
// serUsers -- make update printUsers
// dont push more than one message in outcommands. move to in.
// scrollable chatlog


// TESTS:
// chatlog works fine (wraps, goes up fine)
// commands work fine (incorrect, correct but bad args, correct)

// BUGS:
// unmuting someone changes them to REGULAR. (same with release) -- to fix add condition member variable to users.
// if username has swears and you try to kick, it will change the name to goodwords


char *Servers_master = "MainChannel:";
char *Servers_sub[] = {"Spawn Area", "Lachlans room", "All talk", "Sports and stuff", "Assignment Help"};
char *graphics[] = {" _____       __      ", "|_   _|     / _|     ", "  | | _ __ | |_ ___  ", "  | || '_ \\|  _/ _ \\ ", " _| || | | | || (_) |", " \\___/_| |_|_| \\___/ "};               
// char *graphics[] = {" _   _            ", "| | | |           ", "| |_| | ___ _   _ ", "|  _  |/ _ \\ | | |", "| | | |  __/ |_| |", "\\_| |_/\\___|\\__, |", "             __/ |", "            |___/ "};

#define NEWLINE_OFFSET 26
#define CHAT_LOG_SIZE 4
#define SUB_SIZE 5
#define SUB_OFFSET 5 //distance away from side
#define USERS_OFFSET 3
#define CHAT_SIZE 0.8
#define INFO 0
#define SERV 1
#define CHAT 2

#define OUT 0
#define IN 	1
#define COM 3

#define NCOMMANDS 10
#define NFONTCOMMANDS 11

#define HELP 	1
#define KICK 	2
#define MUTE  	3
#define UNMUTE  4
#define POKE 	5
#define PM 		6
#define EXIT 	7
#define IGNORE 	8
#define TIMEOUT	9
#define RELEASE 10


GUI::GUI(User user) {
	//cant construct windows here because initScreen hasn't been called
	_user = user;
}

GUI::~GUI() {

}

void GUI::initScreen() {
	initscr();
	cbreak();
	// halfdelay(1);
	noecho();
	curs_set(0);
	//STORE USERNAME + user list (request)
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
	// nodelay(message_win,1);

	DATA.reserve(100);
	DATA.push_back("[05:30:12] : Feyre    : Hey everyone!");    
	DATA.push_back("[06:25:15] : Mike     : Oh hey :)");    
	DATA.push_back("[05:30:52] : Tim      : Poop");    
	DATA.push_back("[05:30:19] : Feyre    : Uhh, what?");    

	_users.reserve(15);
	_users.push_back(User("Feyre", SADMIN));
	_users.push_back(User("Tim", REGULAR));
	_users.push_back(User("Mike", REGULAR));
	_users.push_back(User("Liam", SADMIN));
	_users.push_back(User("Johnny", REGULAR));

	start_color();
	init_pair(1, COLOR_RED, 	COLOR_BLACK);
	init_pair(2, COLOR_GREEN, 	COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, 	COLOR_BLACK);
	init_pair(4, COLOR_BLUE, 	COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, 	COLOR_BLACK);
	init_pair(7, COLOR_WHITE, 	COLOR_BLACK);
}

void GUI::resize(void) {
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
	wclear(message_win);									//
	wclear(servers_win);									//
	wclear(chat_win);										//
	wclear(stdscr);										//
    wborder(info_scr, 		'|', '|', '-','-','+','+','+','+');
    wborder(servers_scr, 	'|', '|', '-','-','+','+','+','+');
    wborder(chat_win, 		'|', '|', '-','-','+','+','+','+');			//clear standard screen elsewhere??
    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
    wborder(servers_win, 	'|', '|', '-','-','+','+','+','+');
	wrefresh(stdscr);										//
	wrefresh(chat_win);										//
	wrefresh(message_win);									//
	wrefresh(servers_win);									//
	showScreen(CHAT, 0);	// no selected server??
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

void GUI::printUsers(WINDOW *scr) {
	int ii;
	wclear(scr);
    wborder(scr, 	'|', '|', '-','-','+','+','+','+');
	mvwprintw(scr,1,1, Servers_master);						//change to subservers[a] and add int a.
	for(ii = 0; ii < _users.size(); ii++) {
		if(_users[ii].getPriviledges() > REGULAR) {
			wattron(scr, COLOR_PAIR(5));
		}
		if(_users[ii].getUser() == _user.getUser()) {
			wattron(scr, A_REVERSE);
			// mvwprintw(scr,ii+2,USERS_OFFSET, _users[ii].getUser().c_str());
			// wattroff(scr, A_REVERSE);
		}
		mvwprintw(scr,ii+2,USERS_OFFSET, _users[ii].getUser().c_str());
		wattroff(scr, A_REVERSE);
		wattroff(scr, COLOR_PAIR(5));
	}
	wrefresh(scr);
}


void GUI::test() {

}

void GUI::printTimeout() {

}




































int offset = 0;
//font commands:
// highlight (yellow background, black text)
// colours (red, blue)
void GUI::printChat() {
	int row, col;
	getmaxyx(chat_win, row, col);
	wclear(chat_win);
    wborder(chat_win, 	'|', '|', '-','-','+','+','+','+');
	string fontcommands[] = {"/b{", "/i{", "/u{", "/c{", "/red{", "/green{", "/yellow{", "/blue{", "/magenta{", "/cyan{", "/white{"};

	string message;
	int message_len;



	int ii;
    int m_index;
    int message_lines = 0;

	int lim = DATA.size()-row+2-offset;
	if (lim < 0) {lim = 0;}
 	for(m_index = lim; m_index < DATA.size()-offset; m_index++) {

 		message = DATA[m_index];
		message_len = DATA[m_index].size();

		for(ii = 0; ii < NFONTCOMMANDS; ii++) {
			int font_loc = 1;
			int font_size = fontcommands[ii].size();
			while(font_loc = message.find(fontcommands[ii]) != string::npos) {
				message = message.substr(font_loc+font_size);			// ERROR HERE!!!!!!!!!!!!!!
				message_len -= font_size+1;								//+1 for escape (cant do, what if /b{rgrsgdr)
			}
		}

		if(message_len>col-2) {
		message_lines++;
		message_len -= col-4;
		int temp = message_len/(col-4-NEWLINE_OFFSET);
		message_lines += temp + 1;
		}
		else {message_lines++;}
 	}


 	int shit = 0;
 	int h_index = 1;
	int attempt = 0;
	int jj, command_found;
 	for(m_index = lim; m_index < DATA.size()-offset; m_index++) {
		message = DATA[m_index];
		message_len = DATA[m_index].size();


		if(message.find("/") == string::npos) {														// if no /
			printMessage(message, message_lines, &h_index, &attempt, &message_len, 0, 1);			// print
		}
		else {																						// if /
			int font_loc, font_end;
			int message_len_sofar = 0;
			int prev_len = 0;
			vector<int> current_atr; //wanted stack, but cant check if already in stack (QUEUE = FILA)
			current_atr.reserve(10);

			while((message.find("/") != string::npos) || (message.find("}") != string::npos)) {
				command_found = 0;
				int slash_shorter = 0;
				int bracket_shorter = 0;
				// int loc_b = 
				// int loc_s = 
				if((message.find("/") != string::npos) && (message.find("}") != string::npos)) {
					if(message.find("/") < message.find("}")) {
						font_loc = message.find("/");
						slash_shorter = 1;
					}
					else {
						font_loc = message.find("}");
						bracket_shorter = 1;
					}
				}
				else if(message.find("/") != string::npos) {
					font_loc = message.find("/");
					slash_shorter = 1;
				}
				else if(message.find("}") != string::npos) {
					font_loc = message.find("}");
					bracket_shorter = 1;
				}


				if(slash_shorter) {										// look for command
					for(ii = 0; ii < NFONTCOMMANDS; ii++) {												 
						int font_size = fontcommands[ii].size();

						if(message.substr(font_loc,font_size).find(fontcommands[ii]) != string::npos) {	// if command,
							//if command found
							command_found = 1;
							
							//print all before
							string message_seg = message.substr(0,font_loc);
							message_len_sofar += message_seg.size(); 
							printMessage(message_seg, message_lines, &h_index, &attempt, &message_len_sofar, prev_len, 0);
							prev_len = message_len_sofar;

							//trunk message
							// message = message.erase(0,font_loc+font_size-1);
							message = message.substr(font_loc+font_size);

							//add atr (if not added)
							for(jj = 0; jj < current_atr.size(); jj++) {
								if(ii == current_atr[jj]) {
									jj = -1;
									break;
								}
							}
							if(jj != -1) {
								current_atr.push_back(ii);
								switch(ii) {
									case 0:
										wattron(chat_win, A_BOLD);


										break;
									case 1:
										// wattron(chat_win, A_ITALIC?);
										break;
									case 2:
										wattron(chat_win, A_UNDERLINE);
										break;
									case 3:
										// wattron(chat_win, A_COLOUR?);
										break;
									case 4:
										wattron(chat_win, COLOR_PAIR(1));
										break;

									case 5:
										wattron(chat_win, COLOR_PAIR(2));
										break;

									case 6:
										wattron(chat_win, COLOR_PAIR(3));
										break;

									case 7:
										wattron(chat_win, COLOR_PAIR(4));
										break;

									case 8:
										wattron(chat_win, COLOR_PAIR(5));
										break;

									case 9:
										wattron(chat_win, COLOR_PAIR(6));
										break;

									case 10:
										wattron(chat_win, COLOR_PAIR(7));
										break;
								}
							}
							break;
						}
					}
					if(!command_found) {
						// command not found
						font_loc = message.find("/");
						//print slash

						string message_seg = message.substr(0,font_loc+1);
						message_len_sofar += message_seg.size(); 
						printMessage(message_seg, message_lines, &h_index, &attempt, &message_len_sofar, prev_len, 0);
						prev_len = message_len_sofar;
						//trunk message
						// message = message.erase(0,font_loc+font_size-1);
						message = message.substr(font_loc+1);											//what if / at end?
					}
				}
				else if(bracket_shorter) {								// look for end
					//if end found

					if(!current_atr.empty()) {
						//print all before
						string message_seg = message.substr(0,font_loc);
						message_len_sofar += message_seg.size(); 
						printMessage(message_seg, message_lines, &h_index, &attempt, &message_len_sofar, prev_len, 0);
						prev_len = message_len_sofar;

						//trunk message
						// message = message.erase(0,font_loc+font_size-1);
						message = message.substr(font_loc+1);						//+1 from delim -1 from font_loc balance

						switch(current_atr.back()) {
							case 0:
								wattroff(chat_win, A_BOLD);
								break;
							case 1:
								// wattroff(chat_win, A_ITALIC?);
								break;
							case 2:
								wattroff(chat_win, A_UNDERLINE);
								break;
							case 3:
								// wattroff(chat_win, A_COLOUR?);
								break;
							case 4:
								wattroff(chat_win, COLOR_PAIR(1));
								break;

							case 5:
								wattroff(chat_win, COLOR_PAIR(2));
								break;

							case 6:
								wattroff(chat_win, COLOR_PAIR(3));
								break;

							case 7:
								wattroff(chat_win, COLOR_PAIR(4));
								break;

							case 8:
								wattroff(chat_win, COLOR_PAIR(5));
								break;

							case 9:
								wattroff(chat_win, COLOR_PAIR(6));
								break;

							case 10:
								wattroff(chat_win, COLOR_PAIR(7));
								break;
						}
						current_atr.pop_back();
					}
					else {
						//no current atr

						//print all before
						string message_seg = message.substr(0,font_loc+1);
						message_len_sofar += message_seg.size(); 
						printMessage(message_seg, message_lines, &h_index, &attempt, &message_len_sofar, prev_len, 0);
						prev_len = message_len_sofar;

						//trunk message
						// message = message.erase(0,font_loc+font_size-1);
						message = message.substr(font_loc+1);						//+1 from delim -1 from font_loc balance

					}
				}
			}
			string message_seg = message;
			message_len_sofar += message_seg.size(); 
			printMessage(message_seg, message_lines, &h_index, &attempt, &message_len_sofar, prev_len, 1);
		}
		wattrset(chat_win, A_NORMAL);	// REMOVE FROM VECTOR?
	}
	wrefresh(chat_win);
}




// str, message_lines-attempt, h_index, attempt
//change 1 to message_len_sofar?
void GUI::printMessage(string message, int message_lines, int *h_index, int *attempt, int *message_len/*sofar*/, int prev_len, int endline) {
	// if(message == "") {return;}
	int row, col;
	getmaxyx(chat_win, row, col);


	if((*message_len) > col-2) {
		if(message_lines-(*attempt)<=row-2) {													//
			mvwprintw(chat_win,(*h_index),prev_len+1,(message.substr(0,col-4)+"..").c_str());		//
			(*h_index)++;																		//
		}																						//		tryprint+trunkate
		message = message.substr(col-4);														//		  (*h_index),(*attempt),message, (*message_len)
		(*message_len) -= (col-4);																//			  must be pointers
		(*attempt)++;																			//
		while((*message_len) > col-2-NEWLINE_OFFSET) {
			if(message_lines-(*attempt)<=row-2) {
				mvwprintw(chat_win,(*h_index),NEWLINE_OFFSET+prev_len+1,(message.substr(0, col-4-NEWLINE_OFFSET)+"..").c_str());
				(*h_index)++;
			}
			message = message.substr(col-4-NEWLINE_OFFSET);
			(*message_len) -= (col-4-NEWLINE_OFFSET);
			(*attempt)++;
		}
		if(message_lines-(*attempt)<=row-2) {
			mvwprintw(chat_win,(*h_index),NEWLINE_OFFSET+prev_len+1,message.c_str());
			(*h_index)++;
		}
		(*attempt)++;
	}
	else {
		if(message_lines-(*attempt)<=row-2) {
			mvwprintw(chat_win,(*h_index),prev_len+1,message.c_str());
			(*h_index) += endline;
		}
		(*attempt)++;
	}
}







































void GUI::userInput() {
	if(_user.getPriviledges() == TIMEDOUT) {return;}
	echo();						

	int row, col;
	getmaxyx(message_win, row, col);

	char time_char[11];
	char mess_char[col];			
	wmove(message_win, row/2, (int)(col*0.1));


	// wgetnstr(message_win, mess_char, (int)(col*0.8));

	noecho();						
	string message_str ="";
	string temp = "";
	while(true)	{
		int c = wgetch(message_win);
		if(c == KEY_RESIZE) {
			c = wgetch(message_win);
			resize();
			getmaxyx(message_win, row, col);
			if(message_str.size() >= (int)(col*0.8)) {
				message_str = message_str.substr(0,(int)(col*0.8));
			}
			wclear(message_win);
		    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
			wmove(message_win, row/2, (int)(col*0.1));
			wprintw(message_win,message_str.c_str());
			wrefresh(message_win);
			continue;
		}
		else if(c == KEY_BACKSPACE) {
			message_str = message_str.substr(0,message_str.size()-1);
			wclear(message_win);
		    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
			wmove(message_win, row/2, (int)(col*0.1));
			wprintw(message_win,message_str.c_str());
			wrefresh(message_win);
			// sleep(2);
		}
		else if(c == ERR || c == '\n') {break;}
		else if(message_str.size() >= (int)(col*0.8)) {continue;} 
		else if(!isprint(c)) {
			mRecieved("             <SERVER> : You have entered a non printable character.");
			wmove(message_win, row/2, (int)(col*0.1)+message_str.length());
		}
		else {
			message_str += c;
			waddch(message_win,c);
			// wclear(message_win);
		 //    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
			// wmove(message_win, row/2, (int)(col*0.1));
			// wprintw(message_win,message_str.c_str());
			wrefresh(message_win);
		} 
	}

	// string message_str(mess_char);




	// int ii;
	// for(ii = 0; ii < temp.size(); ii++) {
	// 	if(!isprint(mess_char[ii])) {
	// 		mRecieved("             <SERVER> : You have entered a non printable character.");
	// 		wclear(message_win);	//MAYBE: write own clear function that wont clear border.
	// 	    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
	// 		wrefresh(message_win);
	// 		noecho();
	// 		return;
	// 	}
	// }

	// if(message_str.find_first_of(KEY_RESIZE) != string::npos) {
	// 	resize();
	// }

	
	if(_user.getPriviledges() == 0) {
		wclear(message_win);	//MAYBE: write own clear function that wont clear border.
	    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
		wrefresh(message_win);
		noecho();
		return;
	}

	int swore = checkVulgar(&message_str);
	if(swore) {
		if(_user.getSwears() >= 5) {
			//kick user
			endwin();
			exit(EXIT_SUCCESS);
		}
		else if(_user.getSwears() >= 4) {
			mRecieved("             <SERVER> : This is your final warning, swear again and you will be kicked.");
		}
		else if(_user.getSwears() >= 3) {
			mRecieved("             <SERVER> : Please refrain from profanity.");
		}
	}



	// pad username
	string user_str = _user.getUser();
	user_str.append(8 - user_str.length(), ' ');

	// time
	time_t timer;
	time(&timer);
	struct tm * timeinfo;
	timeinfo = localtime(&timer);
	strftime(time_char,11,"[%H:%M:%S]",timeinfo);
	string time_str(time_char);

	string DATA_str = time_str + " : " + user_str + " : " + message_str;

	wclear(message_win);	//MAYBE: write own clear function that wont clear border.
    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
	wrefresh(message_win);
	noecho();

	//if not null
	if(!command(DATA_str, OUT)) {
		MESSAGE_QUEUE.push(DATA_str);
	}
}














string message_str_g ="";

void GUI::userInputB() {
	if(_user.getPriviledges() == TIMEDOUT) {return;}
	echo();						

	int row, col;
	getmaxyx(message_win, row, col);

	char time_char[11];
	char mess_char[col];			
	wmove(message_win, row/2, (int)(col*0.1));
	wprintw(message_win,message_str_g.c_str());

	noecho();						
	string temp = "";

	int c = wgetch(message_win);

	if(c != '\n') {
		if(c == KEY_RESIZE) {
			c = wgetch(message_win);
			resize();
			getmaxyx(message_win, row, col);
			if(message_str_g.size() >= (int)(col*0.8)) {
				message_str_g = message_str_g.substr(0,(int)(col*0.8));
			}
			wclear(message_win);
		    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
			wmove(message_win, row/2, (int)(col*0.1));
			wprintw(message_win,message_str_g.c_str());
			wrefresh(message_win);
			return;
		}
		else if(c == KEY_BACKSPACE) {
			message_str_g = message_str_g.substr(0,message_str_g.size()-1);
			wclear(message_win);
		    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
			wmove(message_win, row/2, (int)(col*0.1));
			wprintw(message_win,message_str_g.c_str());
			wrefresh(message_win);
			// sleep(2);
		}
		else if(c == KEY_UP) {
			getmaxyx(chat_win, row, col);
			int lim = DATA.size()-row+2;
			if(lim < 0) {lim = 0;}
			if(offset < lim) {
				offset++;
				printChat();
			}
		}
		else if(c == KEY_DOWN) {
			if(offset > 0) {
				offset--;
				printChat();
			}
		}
		else if(c == ERR) {return;}
		else if(message_str_g.size() >= (int)(col*0.8)) {return;} 
		else if(!isprint(c)) {
			mRecieved("             <SERVER> : You have entered a non printable character.");
			wmove(message_win, row/2, (int)(col*0.1)+message_str_g.length());
		}
		else {
			message_str_g += c;
			waddch(message_win,c);
			wrefresh(message_win);
		} 
		return;
	}

	
	if(_user.getPriviledges() == 0) {
		wclear(message_win);	//MAYBE: write own clear function that wont clear border.
	    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
		wrefresh(message_win);
		noecho();
		return;
	}

	int swore = checkVulgar(&message_str_g);
	if(swore) {
		if(_user.getSwears() >= 5) {
			//kick user
			endwin();
			exit(EXIT_SUCCESS);
		}
		else if(_user.getSwears() >= 4) {
			mRecieved("             <SERVER> : This is your final warning, swear again and you will be kicked.");
		}
		else if(_user.getSwears() >= 3) {
			mRecieved("             <SERVER> : Please refrain from profanity.");
		}
	}


	// pad username
	string user_str = _user.getUser();
	user_str.append(8 - user_str.length(), ' ');

	// time
	time_t timer;
	time(&timer);
	struct tm * timeinfo;
	timeinfo = localtime(&timer);
	strftime(time_char,11,"[%H:%M:%S]",timeinfo);
	string time_str(time_char);

	string DATA_str = time_str + " : " + user_str + " : " + message_str_g;

	wclear(message_win);	//MAYBE: write own clear function that wont clear border.
    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
	wrefresh(message_win);
	noecho();

	//if not null
	if(!command(DATA_str, OUT)) {
		MESSAGE_QUEUE.push(DATA_str);
	}
	message_str_g = "";
}






















// replace words or give random &%#$*^?
// give a vector and command to pushback new bandwords??
int GUI::checkVulgar(string *message) {
	int ii, jj = 0;
	string badwords[] =  {"cunt",		"fucker", "fuck", "dick"    , "slut"  , "shit", "ass", "cock"  , "pussy" , "fag"   , "bitch"         , "wanker"   , "whore"};
	string goodwords[] = {"croissant", 	"Tucker", "duck", "wee-wee" , "muppet", "poop", "bum", "donger", "kitten", "wizard", "b with an itch", "gentleman", "loon"};
	int size = 13;
	int loc;

	for(ii = 0; ii < size; ii++) {
		if((loc = upperCase(*message).find(upperCase(badwords[ii]))) != string::npos) {
			(*message).replace(loc,badwords[ii].size(),goodwords[ii]);
			jj = 1; // change to ++ if you want n. of badwords
		}
	}
	if(jj) {_user.incSwears();}
	return jj;
}

// REDO, make to lower? then can avoid uppering badwords.
string GUI::upperCase(string message) {
  for (string::iterator it = message.begin(); it != message.end(); ++ it)
    *it = toupper(*it);
  return message;
}
























// HELP - /help
//  - all
//
//
// KICK - /kick <user/all> <reason?>
//  - add reason
//  - remove user from userList
//  - show kick screen
//  - tell other modules to kick <user>
//  prints: 
// 		<user1> has kicked <user2>.		-- succsess
// 		Please enter the target user.	-- failure (no target)				not enough input args?
// 		<user> is not online.			-- failure (target offline)
//
//
// BAN - /ban <user> <time min> <reason>
//  - most
//
//
// POKE - /poke <user>
//  - done
//  prints: 
// 		You have uhh.. poked, yourself?	-- succsess (self)
// 		You have poked <user>!			-- succsess
// 		Please enter the target user.	-- failure (no target)
// 		<user> is not online.			-- failure (target offline)
//
//
// PM - /pm <user>
//  - all
//
//
// FONTS
// if they put /bold isntead of /b< let them know?
// ctrl + b? (would have to ensure all premessage stuff printed normally)
//
// IDEAS: 
// exit
// mute


// /help
// /kick <user/all> <reason?>
// /mute <user> - time,reason?   // /ban  <user> <time mins> <reason> -- mute
// /poke <user> 
// /pm <user>
// /exit
// /block <user>	-- block = ignore = localmute
// /unblock <user>

// make non case sensitive? or tab complete?

// push commands like mute and timeout to let everyone know

// user polymorphism on all the checks to sort it out

bool GUI::command(string message, int out_in) {
	// message = message.substr(24); 		//bad to edit? maybe only give it the message part


	// INITIALIZE VARIABLES
	int ii, jj = 0;
	string this_user = _user.getUser();
	string commands[] = {"/help", "/kick", "/mute", "/unmute", "/poke", "/pm", "/exit", "/ignore", "/timeout", "/release"};
	// string fontcommands[] = {"/b{", "/i{", "/u{", "/c{"};	//<b>message<b/>
	string fontcommands[] = {"/b{", "/i{", "/u{", "/c{", "/red{", "/green{", "/yellow{", "/blue{", "/magenta{", "/cyan{", "/white{"};
	string command_str;
	string user_str;
	string arg_str;
	string arg_str2;
	string temp;

	// EXTRACT TOKENS
	stringstream ss(message);
    ss >> temp;					//time
    ss >> temp;					// :
    ss >> user_str;				//user
    ss >> temp;					// :
    ss >> command_str;			//command
    ss >> arg_str;				//arg 			WHAT IF NO ARG?

	if(!ss.eof()) {
		arg_str2 = "";
		while(!ss.eof()) {
			ss >> temp;
			arg_str2 += temp + " ";
		}
	}

    // INITIAL CHECK FOR COMMAND
	if(command_str.substr(0,1) != "/") return 0;			//UPDATE LATER (incase text style changes (time:name:message));

	// CHECK FOR FONT COMMANDS
	for(ii = 0; ii < NFONTCOMMANDS; ii++) {
		if(command_str.substr(0,fontcommands[ii].size()) == fontcommands[ii]) {
			return 0;
		}
	}

	// CHECK FOR COMMANDS
	for(ii = 0; ii < NCOMMANDS; ii++) {
		if(command_str == commands[ii]) {
			jj = ii+1;
			break;
		}
	}

	// CHECK USER IS ONLINE -- move to command specific if commands have arg1 as not user
	User arg_obj;
    int user_exists = 0;
	for(ii = 0; ii < _users.size(); ii++) {
		if(arg_str == _users[ii].getUser()) {
			user_exists = 1;
			arg_obj = _users[ii];
		}
	}


	// USER SENT COMMAND
	if(out_in == OUT) {		
		switch(jj) {

			case HELP:
				//disp help screen
				return 1;



			case KICK:
				if(arg_str == "all") {
					MESSAGE_QUEUE.push(message);
					// MESSAGE_QUEUE.push("             <SERVER> : " + this_user + " has kicked " + arg_str + ".");
				}
				else if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				else if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You have tried to, uhh.. kick yourself?");
					return 1;
				}
				else if(_user.getPriviledges() <= arg_obj.getPriviledges()) {
					mRecieved("             <SERVER> : You cannot kick a superior or matching rank.");
					return 1;
				}
				else if(user_exists) {
					MESSAGE_QUEUE.push(message);
					// MESSAGE_QUEUE.push("             <SERVER> : " + this_user + " has kicked " + arg_str + ".");
				}
				return 1;



				// you cant kick all


			case MUTE:
				//checkCommand(MUTE, user_exists, thi)
				if(arg_str == "all") {
					MESSAGE_QUEUE.push(message);
				}
				else if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				else if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You cannot mute yourself.");
					return 1;
				}	
				else if(_user.getPriviledges() <= arg_obj.getPriviledges()) {
					mRecieved("             <SERVER> : You cannot mute a superior or matching rank.");
					return 1;
				}
				else if(user_exists) {
					MESSAGE_QUEUE.push(message);
				}
				return 1;





			case UNMUTE:
				if(arg_str == "all") {
					MESSAGE_QUEUE.push(message);
				}
				else if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				else if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You cannot unmute yourself.");
					return 1;
				}	
				// else if(_user.getPriviledges() <= arg_obj.getPriviledges()) {
				// 	mRecieved("             <SERVER> : You cannot mute a superior or matching rank.");
				// 	return 1;
				// }
				else if(user_exists) {
					MESSAGE_QUEUE.push(message);
				}

				return 1;









			case POKE:
				if(user_exists) {
					MESSAGE_QUEUE.push(message);
					if (arg_str == this_user) 	{mRecieved("             <SERVER> : You have uhh.. poked, yourself?");}
					else 					{mRecieved("             <SERVER> : You have poked " + arg_str + "!");}
				}
				else if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
				else if (arg_str!="")		{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
				return 1;








			case PM:
				if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				// else if(_user.getPriviledges() < ADMIN) {
				// 	mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
				// 	return 1;
				// }
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You cannot pm yourself.");
					return 1;
				}	
				// else if(_user.getPriviledges() <= arg_obj.getPriviledges()) {
				// 	mRecieved("             <SERVER> : You cannot mute a superior or matching rank.");
				// 	return 1;
				// }
				else if(!ss.eof()) {
					MESSAGE_QUEUE.push(message);
				}
				return 1;











			case EXIT:
				MESSAGE_QUEUE.push(message);
				return 1;












			case IGNORE:
				if(arg_str == "all") {
					MESSAGE_QUEUE.push(message);
				}
				else if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				else if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You cannot mute yourself.");
					return 1;
				}	
				else if(_user.getPriviledges() <= arg_obj.getPriviledges()) {
					mRecieved("             <SERVER> : You cannot mute a superior or matching rank.");
					return 1;
				}
				else if(user_exists) {
					MESSAGE_QUEUE.push(message);
				}

				return 1;















			case TIMEOUT:
				if(arg_str == "all") {
					MESSAGE_QUEUE.push(message);
				}
				else if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				else if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You cannot timeout yourself.");
					return 1;
				}	
				else if(_user.getPriviledges() <= arg_obj.getPriviledges()) {
					mRecieved("             <SERVER> : You cannot timeout a superior or matching rank.");
					return 1;
				}
				else if(user_exists) {
					MESSAGE_QUEUE.push(message);
				}

				if(!ss.eof()) {
					arg_str2 = "";
					while(!ss.eof()) {
						ss >> temp;
						arg_str2 += temp + " ";
					}
					MESSAGE_QUEUE.push 				  		  ("             <SERVER> : Reason: " + arg_str2);
				}
				return 1;












			case RELEASE:
				if(arg_str == "all") {
					MESSAGE_QUEUE.push(message);
				}
				else if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				else if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You cannot untimeout yourself.");
					return 1;
				}	
				// else if(_user.getPriviledges() <= arg_obj.getPriviledges()) {
				// 	mRecieved("             <SERVER> : You cannot mute a superior or matching rank.");
				// 	return 1;
				// }
				else if(user_exists) {
					MESSAGE_QUEUE.push(message);
				}

				return 1;







			default:
				cout << '\a';
				mRecieved("             <SERVER> : Command not recognized.");
				return 1;
		}
















	} //USER RECIEVED COMMAND
	else if(out_in == IN) {		
		switch(jj) {
			case HELP:
				//should never see
				return 1;


			case KICK:
				if((arg_str == this_user) || ((arg_str == "all")&&(_user.getPriviledges() < ADMIN))) {
					// show kick screen
					// call kick function
					endwin();
					exit(EXIT_SUCCESS);
					return 1;
				}
				mRecieved("             <SERVER> : " + user_str + " has kicked " + arg_str + ".");
				if(arg_str2 != "") {
					mRecieved("             <SERVER> : Reason: " + arg_str2);
				}

				removeUser(arg_str);									
				return 1;


			case MUTE:
				if((arg_str == this_user) || ((arg_str == "all")&&(_user.getPriviledges() < ADMIN))) {
					_user.setPriviledges(SPECTATOR);
				}
				mRecieved("             <SERVER> : " + user_str + " has muted " + arg_str + ".");
				if(arg_str2 != "") {
					mRecieved("             <SERVER> : Reason: " + arg_str2);
				}
				return 1;


			case UNMUTE:
				if((arg_str == this_user) || ((arg_str == "all")&&(_user.getPriviledges() < ADMIN))) {
					_user.setPriviledges(REGULAR);
				}
				mRecieved("             <SERVER> : " + user_str + " has unmuted " + arg_str + ".");
				return 1;


			case POKE:
				if(arg_str == this_user) {
					cout << '\a';
					if(arg_str != user_str) {mRecieved("             <SERVER> : You have been poked by " + user_str);}
				}
				return 1;	



			case PM:
				if(arg_str == this_user) {
					mRecieved("/yellow{/b{<" + user_str + "> : " + arg_str2 + "}}");
				}
				else if (user_str == this_user) {
					mRecieved("/cyan{/b{<" + user_str + "> : " + arg_str2 + "}}");
				}
				return 1;






			case EXIT:
				if(user_str == this_user) {
					endwin();
					exit(EXIT_SUCCESS);
				}
				removeUser(arg_str);									
				return 1;






			case IGNORE:
				// MESSAGE_QUEUE.push(message);
				return 1;







			case TIMEOUT:
				if((arg_str == this_user) || ((arg_str == "all")&&(_user.getPriviledges() < ADMIN))) {
					_user.setPriviledges(TIMEDOUT);
					printTimeout();
					// MESSAGE_QUEUE.push 				  		  ("             <SERVER> : " + user_str + " has unmuted " + arg_str + ".");
				}
				mRecieved("             <SERVER> : " + user_str + " has put " + arg_str + " in timeout.");
				if(arg_str2 != "") {
					mRecieved("             <SERVER> : Reason: " + arg_str2);
				}
				return 1;



			case RELEASE:
				if((arg_str == this_user) || ((arg_str == "all")&&(_user.getPriviledges() < ADMIN))) {
					_user.setPriviledges(REGULAR);
					printChat();
					// MESSAGE_QUEUE.push 				  		  ("             <SERVER> : " + user_str + " has unmuted " + arg_str + ".");
				}
				mRecieved("             <SERVER> : " + user_str + " has released " + arg_str + " from timeout.");
				return 1;	
		}
	}
	return 1;
}







void GUI::removeUser(string user) {
	int ii;
	for(ii = 0; ii < _users.size(); ii++) {							
		if(user == _users[ii].getUser()) {
			_users.erase(_users.begin()+ii);
			break;
		}	
	}	
	if(user == "all") {
		for(ii = 0; ii < _users.size(); ii++) {							
			if(_users[ii].getPriviledges() < ADMIN) {					//USERLIST MUST BE OBJECTS
				_users.erase(_users.begin()+ii);
				ii--;
			}
		}	
	}
	printUsers(servers_win);	
}




// only called when someone connects/disconnects/iskicked/isbanned
void GUI::updateUsers(vector<User> users) {
	_users = users;
	printUsers(servers_win);
}












string GUI::mRequested() {	//check colour 
	if(MESSAGE_QUEUE.empty()) {return "/empty";}
	string message = MESSAGE_QUEUE.front();
	MESSAGE_QUEUE.pop();
	return message;
}

void GUI::mRecieved(string message) {
	if(!command(message, IN)) {
		DATA.push_back(message);
		if(_user.getPriviledges() != TIMEDOUT) {
			printChat();
		}
	}
}













void GUI::showScreen(int a, int b) {
	switch(a) {
		case INFO:
			int row, col;
			getmaxyx(info_scr,row,col);
			printAscii(info_scr, graphics, 6, 21, (int)(row*0.1), (col-21)/2);
			mvwprintw(info_scr, row-20,  4, "/b{TEXT}  --  bold font");
			mvwprintw(info_scr, row-19,  4, "/u{TEXT}  --  underlined font");
			mvwprintw(info_scr, row-18, 4, 	"/i{TEXT}  --  italic font (not currently implemented)");
			mvwprintw(info_scr, row-17, 4, 	"/c{TEXT}  --  placeholder");
			mvwprintw(info_scr, row-16, 4, 	"/colour{TEXT}  --  coloured font (red, green, blue, yellow, magenta, cyan, white)");

			mvwprintw(info_scr, row-12, 4, 	"/help                           --  reopens this screen");
			mvwprintw(info_scr, row-11, 4, 	"/kick    <user/all> <reason*>   --  kicks user");
			mvwprintw(info_scr, row-10, 4, 	"/mute    <user/all> <reason*>   --  mutes user");
			mvwprintw(info_scr, row-9, 4, 	"/unmute  <user/all>             --  unmutes user");
			mvwprintw(info_scr, row-8, 4, 	"/poke    <user>                 --  pokes user");
			mvwprintw(info_scr, row-7, 4, 	"/pm      <user> <message>       --  sends user a private message");
			mvwprintw(info_scr, row-6, 4, 	"/exit                           --  exits the program");
			mvwprintw(info_scr, row-5, 4, 	"/ignore  <user>                 --  you will not recieve messages from this user");
			mvwprintw(info_scr, row-4, 4, 	"/timeout <user> <reason*>       --  sends user to his own room");
			mvwprintw(info_scr, row-3, 4, 	"/release <user>                 --  brings user back to main chat");

			wrefresh(info_scr);
			break;

		case SERV:
			printServers(servers_scr, b);
			break;

		case CHAT:
			// printServers(servers_win, b);
			printUsers(servers_win);
			wrefresh(message_win);
			printChat();
			curs_set(1);
			break;
	}
}

int GUI::selectServer() {
	showScreen(SERV, 0);
	int c = 'o';
	int selection = 0, choice = 0;//, ii = 0;
	while(1) {	
		// ii++;
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
		}
		// mvwprintw(servers_scr, 0, 0, "%d", ii);
		showScreen(SERV, selection);
	}	
}





















void GUI::Routine() {
	showScreen(INFO, 1);
	wgetch(info_scr);
	int selectedServer = selectServer();
	showScreen(CHAT, selectedServer);

	while(1) {
		userInputB();
		string MESSAGE = mRequested();
		if(MESSAGE == "/empty") {continue;}
		mRecieved(MESSAGE);

		MESSAGE = mRequested();
		if(MESSAGE == "/empty") {continue;}
		mRecieved(MESSAGE);

		MESSAGE = mRequested();
		if(MESSAGE == "/empty") {continue;}
		mRecieved(MESSAGE);

		MESSAGE = mRequested();
		if(MESSAGE == "/empty") {continue;}
		mRecieved(MESSAGE);

		MESSAGE = mRequested();
		if(MESSAGE == "/empty") {continue;}
		mRecieved(MESSAGE);
	}
}



	//MAIN:
	//loop for user input
	//if found send to queue
	//await recieved message

	//BUGS:
	//cpy+pste 2ce kills (segfault)



	//find max username size?
	//have option for seconds/noseconds

	//QUESTIONS:
	//how and where is users stored
	//how should userinput be called
	//user limit?



// Finalise user list/user storage + user types
// Implement font commands 
// Make user input non blocking
