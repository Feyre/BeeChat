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
// setUser
// serUsers -- make update printUsers
// make userin nonblocking (thread)
// finish commands
// permanent font commands
// scrollable chatlog
// improve userinput line
// set profanity filter locally?
// check for valid character set (no asian chars or ^[[a stuff)
// fix bracket check in font commands
// make more robust for stupid user


// TESTS:
// chatlog works fine (wraps, goes up fine)
// commands work fine (incorrect, correct but bad args, correct)

// BUGS:
// textwrap - charsets (hit numkeys)
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
#define IN 1

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
	_users.push_back(User("Tim", SADMIN));
	_users.push_back(User("Mike", SADMIN));
	_users.push_back(User("Liam", SADMIN));
	_users.push_back(User("Johnny", SADMIN));

	start_color();
	init_pair(1, COLOR_RED, 	COLOR_BLACK);
	init_pair(2, COLOR_GREEN, 	COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, 	COLOR_BLACK);
	init_pair(4, COLOR_BLUE, 	COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, 	COLOR_BLACK);
	init_pair(7, COLOR_WHITE, 	COLOR_BLACK);
}

	// COLOR_BLACK   0
	// COLOR_RED     1
	// COLOR_GREEN   2
	// COLOR_YELLOW  3
	// COLOR_BLUE    4
	// COLOR_MAGENTA 5
	// COLOR_CYAN    6
	// COLOR_WHITE   7









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
	mvwprintw(scr,1,1, Servers_master);
	for(ii = 0; ii < _users.size(); ii++) {
		if(_users[ii].getUser() == _user.getUser()) {
			wattron(scr, A_REVERSE);
			mvwprintw(scr,ii+2,USERS_OFFSET, _users[ii].getUser().c_str());
			wattroff(scr, A_REVERSE);
		}
		else {
			mvwprintw(scr,ii+2,USERS_OFFSET, _users[ii].getUser().c_str());
		}
	}
	wrefresh(scr);
}


void GUI::test() {

}







































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

	int lim = DATA.size()-row+2;
	if (lim < 0) {lim = 0;}
 	for(m_index = lim; m_index < DATA.size(); m_index++) {

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
 	for(m_index = lim; m_index < DATA.size(); m_index++) {
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







































// string mess_str = "";				//SWAP0

void GUI::userInput() {
	char mess_char[80];					//SWAP1
	echo();								//SWAP1
	// nodelay(message_win,1);			//SWAP0

	char time_char[11];
	int row, col;
	getmaxyx(message_win, row, col);
	wmove(message_win, row/2, (int)(col*0.1));
	wgetstr(message_win, mess_char);	//SWAP1

	// char c = wgetch(message_win);	//SWAP0
	// if(c == ERR) {return;}			//SWAP0
	// mess_str += c;					//SWAP0
	// wprintw(message_win, mess_str.c_str());
	// if(c != '\n') {return;}			//SWAP0


	string message_str(mess_char);	//usless, change the name of one of them		//SWAP1
	// string message_str(mess_str);												//SWAP0


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

	if(_user.getPriviledges() == 0) {
		wclear(message_win);	//MAYBE: write own clear function that wont clear border.
	    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
		wrefresh(message_win);
		noecho();
		return;
	}


	// pad username
	string user_str = _user.getUser();
	user_str.append(8 - user_str.length(), ' ');

	// 
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
	// mess_str = "";															//SWAP0
	// string messy = MESSAGE_QUEUE.front();
	// mvwprintw(chat_win, 6,1, DATA_str.c_str());
	// mvwprintw(chat_win, 7,1, messy.c_str());
	// wrefresh(chat_win);
	// sleep(2);
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

	// user_str = user_str + //spaces depending on size
	// user_str.append(8 - user_str.length(), ' ');
	// arg_str.append(8 - arg_str.length(), ' ');
	//errors when input is too long

	// CHECK USER IS ONLINE -- move to command specific if commands have arg1 as not user
	User arg_obj;
    int user_exists = 0;
	for(ii = 0; ii < _users.size(); ii++) {
		if(arg_str == _users[ii].getUser()) {
			user_exists = 1;
			arg_obj = _users[ii];
		}
	}

	// DEBUG CODE
    // mvwprintw(message_win,0,0,(USER + " : " + user_str + " " + command_str + " " + arg_str).c_str());
    // mvwprintw(message_win,1,0,"%d, %d", user_exists,arg_str == USER);
    // wrefresh(message_win);
    // sleep(1);



	// USER SENT COMMAND
	if(out_in == OUT) {		
		switch(jj) {

			case HELP:
				//disp help screen
				return 1;



			case KICK:
				if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				else if(_user.getPriviledges() < arg_obj.getPriviledges()) {
					mRecieved("             <SERVER> : You cannot kick a superior or matching rank.");
					return 1;
				}
				else if((user_exists) || (arg_str == "all")) {
					MESSAGE_QUEUE.push(message);
					MESSAGE_QUEUE.push 				  		  ("             <SERVER> : " + this_user + " has kicked " + arg_str + ".");
					if (arg_str == this_user) 		{mRecieved("             <SERVER> : You have uhh.. kicked, yourself?");}			//DELETE?
				}
				else if (arg_str=="")				{mRecieved("             <SERVER> : Please enter the target user.");}
				else if (arg_str!="")				{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
				if(!ss.eof()) {
					arg_str2 = "";
					while(!ss.eof()) {
						ss >> temp;
						arg_str2 += temp + " ";
					}
					MESSAGE_QUEUE.push 				  		  ("             <SERVER> : Reason: " + arg_str2);
				}
				return 1;






			case MUTE:
				//checkCommand(MUTE, user_exists, thi)
				if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				else if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				else if(_user.getPriviledges() < arg_obj.getPriviledges()) {
					mRecieved("             <SERVER> : You cannot mute a superior or matching rank.");
					return 1;
				}
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You cannot mute yourself.");
					return 1;
				}	

				if((user_exists) || (arg_str == "all")) {
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





			case UNMUTE:
				if(!user_exists) {
					if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
					else if (arg_str!="")	{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
					return 1;
				}
				else if(_user.getPriviledges() < ADMIN) {
					mRecieved("             <SERVER> : Insufficient priviledges. (ADMIN only)");
					return 1;
				}
				// else if(_user.getPriviledges() < arg_obj.getPriviledges()) {
				// 	mRecieved("             <SERVER> : You cannot mute a superior or matching rank.");
				// 	return 1;
				// }
				else if (arg_str == this_user) {
					mRecieved("             <SERVER> : You cannot unmute yourself.");
					return 1;
				}	
						
				if((user_exists) || (arg_str == "all")) {
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









			case POKE:
				if(user_exists) {
					MESSAGE_QUEUE.push(message);
					if (arg_str == this_user) 	{mRecieved("             <SERVER> : You have uhh.. poked, yourself?");}
					else 					{mRecieved("             <SERVER> : You have poked " + arg_str + "!");}
				}
				else if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
				else if (arg_str!="")		{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
				//check if user is on (in userList)
				//send poke request to all clients (throw in queue)
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
				// else if(_user.getPriviledges() < arg_obj.getPriviledges()) {
				// 	mRecieved("             <SERVER> : You cannot mute a superior or matching rank.");
				// 	return 1;
				// }
				// else if (arg_str == this_user) {
				// 	mRecieved("             <SERVER> : You cannot pm yourself.");
				// 	return 1;
				// }	
				else if(!ss.eof()) {
					MESSAGE_QUEUE.push(message);
				}
				return 1;











			case EXIT:
				// MESSAGE_QUEUE.push(message);
				return 1;

			case IGNORE:
				// MESSAGE_QUEUE.push(message);
				return 1;

			case TIMEOUT:
				// MESSAGE_QUEUE.push(message);
				return 1;

			case RELEASE:
				// MESSAGE_QUEUE.push(message);
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
				}

				for(ii = 0; ii < _users.size(); ii++) {							
					if(arg_str == _users[ii].getUser()) {
						_users.erase(_users.begin()+ii);
						break;
					}	
				}	

				if(arg_str == "all") {
					for(ii = 0; ii < _users.size(); ii++) {							
						if(_users[ii].getPriviledges() < ADMIN) {					//USERLIST MUST BE OBJECTS
							_users.erase(_users.begin()+ii);
							ii--;
						}
					}	
				}
				printUsers(servers_win);										
				return 1;


			case MUTE:
				if((arg_str == this_user) || ((arg_str == "all")&&(_user.getPriviledges() < ADMIN))) {
					_user.setPriviledges(SPECTATOR);
					// MESSAGE_QUEUE.push 				  		  ("             <SERVER> : " + user_str + " has muted " + arg_str + ".");
				}
				mRecieved("             <SERVER> : " + user_str + " has muted " + arg_str + ".");
				return 1;


			case UNMUTE:
				if((arg_str == this_user) || ((arg_str == "all")&&(_user.getPriviledges() < ADMIN))) {
					_user.setPriviledges(SPECTATOR);
					// MESSAGE_QUEUE.push 				  		  ("             <SERVER> : " + user_str + " has unmuted " + arg_str + ".");
				}
				mRecieved("             <SERVER> : " + user_str + " has muted " + arg_str + ".");
				return 1;


			case POKE:
				if(arg_str == this_user) {
					cout << '\a';
					if(arg_str != user_str) {mRecieved("             <SERVER> : You have been poked by " + user_str);}
				}
				//check if user matches target
				//if so, ding and print locally
				return 1;	



			case PM:
				if(arg_str == this_user) {
					arg_str2 = "";
					while(!ss.eof()) {
						ss >> temp;
						arg_str2 += temp + " ";
					}
					mRecieved("/yellow{/b{<" + user_str + "> : " + arg_str2 + "}}");
				}
				return 1;






			case EXIT:
				// MESSAGE_QUEUE.push(message);
				return 1;

			case IGNORE:
				// MESSAGE_QUEUE.push(message);
				return 1;

			case TIMEOUT:
				// MESSAGE_QUEUE.push(message);
				return 1;

			case RELEASE:
				// MESSAGE_QUEUE.push(message);
				return 1;	
		}
	}
	return 1;
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
		printChat();
	}
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
		userInput();
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
