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
#include "GUI.h"
using namespace std;

// TODO:
// setUser
// serUsers -- make update printUsers
// make userin nonblocking
// finish commands
// print font commands
// scrollable chatlog
// check for valid character set (no asian chars or ^[[a stuff)
// make more robust for stupid user


// TESTS:
// chatlog works fine (wraps, goes up fine)
// commands work fine (incorrect, correct but bad args, correct)

// BUGS:
// textwrap - charsets (hit numkeys)


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



GUI::GUI() {

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
	USER = "Feyre";
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

	USERS.reserve(15);
	USERS.push_back("Feyre");
	USERS.push_back("Tim");
	USERS.push_back("Mike");
	USERS.push_back("Liam");
	USERS.push_back("Johnny");
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
	mvwprintw(scr,1,1, Servers_master);
	for(ii = 0; ii < USERS.size(); ii++) {
		if(USERS[ii] == USER) {
			wattron(scr, A_REVERSE);
			mvwprintw(scr,ii+2,USERS_OFFSET, USERS[ii].c_str());
			wattroff(scr, A_REVERSE);
		}
		else {
			mvwprintw(scr,ii+2,USERS_OFFSET, USERS[ii].c_str());
		}
	}
	wrefresh(scr);
}














//loop through printing each line
//if line to long, print what you can
//then print the last bit on the next line
//repeat till printed
//if there is a spaced line, you need to start one line later.
//calc how much later first.

void GUI::printChat() {
	int row, col;
	getmaxyx(chat_win, row, col);
	wclear(chat_win);
    wborder(chat_win, 	'|', '|', '-','-','+','+','+','+');





    // GET NUMBER OF LINES NEEDED TO SHOW 'fittable' CHATLOG
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

	// CALCULATE NEW LIMIT (to start screen printing at) -- DELETE
	//incriment limit by lines-row+2 (should be zero unless extra line)
	int lim = DATA.size()-row+2;	
	if (lim < 0) {lim = 0;}
	lim += lines-row+2;
	if (lim < 0) {lim = 0;}

	ii = DATA.size()-row+2;
	if (ii < 0) {ii = 0;}


	// START EVALUATING FROM chatlim, ONLY PRINT IF THERES ROOM. 		(kk = lines tried to print, jj = lines printed, ii = data)
	// ii = message index, starting at n.offset.mess.fit
	// jj = height index,	1, to whatever -- also used to show lines printed (jj-row+2 = lines avaliable to print)
	// lim = n.offset.lines.fit
	// run through all messages that could fit
	// only print if n.of avaliable lines >= room
	// only print if n.of lines left to print <= room
	// n. of lines left to print = lines-kk (lines -lines tried to print)
	// lines-jj wont work because that would change height when you need to ++ regardless of if it works
	int kk = 0;
	for(ii; ii < DATA.size(); ii++) {
		message = DATA[ii];
		message_len = DATA[ii].size();

		if(message_len > col-2) {
			if(lines-kk<=row-2) {													//
				mvwprintw(chat_win,jj,1,(message.substr(0,col-4)+"..").c_str());	//
				jj++;																//
			}																		//		tryprint+casterate
			message = message.substr(col-4);										//		  jj,kk,message, message_len
			message_len -= (col-4);													//			  must be pointers
			kk++;																	//
			while(message_len > col-2-NEWLINE_OFFSET) {
				if(lines-kk<=row-2) {
					mvwprintw(chat_win,jj,NEWLINE_OFFSET+1,(message.substr(0, col-4-NEWLINE_OFFSET)+"..").c_str());
					jj++;
				}
				message = message.substr(col-4-NEWLINE_OFFSET);
				message_len -= (col-4-NEWLINE_OFFSET);
				kk++;
			}
			if(lines-kk<=row-2) {
				mvwprintw(chat_win,jj,NEWLINE_OFFSET+1,message.c_str());
				jj++;
			}
			kk++;
		}
		else {
			if(lines-kk<=row-2) {
				mvwprintw(chat_win,jj,1,message.c_str());
				jj++;
			}
			kk++;
		}
	}
	wrefresh(chat_win);
}
// FINAL PROCESS:
// calc n. of fittable lines
// calc n. offset for lines
// calc n. offset for messages
// try and print all values between n.o.messages and DATA.end()
// only print the values if (n. off lines left <= room)

//BUGS+FIXES
// Text can print below screen if wrapped (Calc n. of lines)
// Text can print below screen at start if wrapped (add lines and lim seperatly)
// Wrapped text dissapears all at once ()


















// string mess_str = "";				//SWAP0

void GUI::userInput() {
	char mess_char[80];					//SWAP1
	echo();								//SWAP1
	// nodelay(message_win,1);			//SWAP0

	curs_set(1);
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


	string MESSAGE(mess_char);	//usless, change the name of one of them		//SWAP1
	// string MESSAGE(mess_str);												//SWAP0
	string user_str = USER;
	user_str.append(8 - user_str.length(), ' ');

	time_t timer;
	time(&timer);
	struct tm * timeinfo;
	timeinfo = localtime(&timer);
	strftime(time_char,11,"[%H:%M:%S]",timeinfo);
	string TIME(time_char);

	string DATA_str = TIME + " : " + user_str + " : " + MESSAGE;

	wclear(message_win);	//MAYBE: write own clear function that wont clear border.
    wborder(message_win, 	'|', '|', '-','-','+','+','+','+');
	wrefresh(message_win);
	curs_set(0);
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
























#define NCOMMANDS 4
#define NFONTCOMMANDS 4
#define HELP 1
#define KICK 2
#define BAN  3
#define POKE 4


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
// ctrl + b?
//
// IDEAS: 
// exit
// mute



bool GUI::command(string message, int out_in) {
	// message = message.substr(24); 		//bad to edit? maybe only give it the message part


	// INITIALIZE VARIABLES
	int ii, jj = 0;
	string commands[] = {"/help", "/kick", "/ban", "/poke"};
	string fontcommands[] = {"/b{", "/i{", "/u{", "/c{"};	//<b>message<b/>
	string command_str;
	string user_str;
	string arg_str;
	string temp;

	// EXTRACT TOKENS
	stringstream ss(message);
    ss >> temp;					//time
    ss >> temp;					// :
    ss >> user_str;				//user
    ss >> temp;					// :
    ss >> command_str;			//command
    ss >> arg_str;				//arg

    // INITIAL CHECK FOR COMMAND
	if(command_str.substr(0,1) != "/") return 0;			//UPDATE LATER (incase text style changes (time:name:message));

	// CHECK FOR FONT COMMANDS
	for(ii = 0; ii < NFONTCOMMANDS; ii++) {
		if(command_str.substr(0,3) == fontcommands[ii]) {
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
    int user_exists = 0;
	for(ii = 0; ii < USERS.size(); ii++) {
		if(arg_str == USERS[ii]) {
			user_exists = 1;
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
				if((user_exists) || (arg_str == "all")) {
					MESSAGE_QUEUE.push(message);
					MESSAGE_QUEUE.push 				  ("             <SERVER> : " + USER + " has kicked " + arg_str + ".");
					if (arg_str == USER) 	{mRecieved("             <SERVER> : You have uhh.. kicked, yourself?");}			//DELETE?
				// else 					{mRecieved("             <SERVER> : You have kicked " + arg_str + "!");}
				}
				else if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
				else if (arg_str!="")		{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
				//check if user is on (in userList)
				//send kickUser request to all clients (throw in queue)
				return 1;


			case BAN:
				// MESSAGE_QUEUE.push(message);
				return 1;


			case POKE:
				if(user_exists) {
					MESSAGE_QUEUE.push(message);
					if (arg_str == USER) 	{mRecieved("             <SERVER> : You have uhh.. poked, yourself?");}
					else 					{mRecieved("             <SERVER> : You have poked " + arg_str + "!");}
				}
				else if (arg_str=="")		{mRecieved("             <SERVER> : Please enter the target user.");}
				else if (arg_str!="")		{mRecieved("             <SERVER> : " + arg_str + " is not online.");}
				//check if user is on (in userList)
				//send poke request to all clients (throw in queue)
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
				if((arg_str == USER) || (arg_str == "all")) {
					// show kick screen
					endwin();
					exit(EXIT_SUCCESS);
				}
				//check if user matches arg
				//if so, kick and print what happened (who kicked who) and why?
				//remove from user list

				for(ii = 0; ii < USERS.size(); ii++) {							//
					if(arg_str == USERS[ii]) {USERS.erase(USERS.begin()+ii);}	// removeUser func?
				}																// 
				printUsers(servers_win);										//
				return 1;


			case BAN:
				//if user is server, add ban.
				return 1;


			case POKE:
				if(arg_str == USER) {
					cout << '\a';
					if(arg_str != user_str) {mRecieved("             <SERVER> : You have been poked by " + user_str);}
				}
				//check if user matches target
				//if so, ding and print locally
				return 1;		
		}
	}
	return 1;
}












// only called when someone connects/disconnects/iskicked/isbanned
void GUI::updateUsers(vector<string> users) {
	USERS = users;
	printUsers(servers_win);
}

// only called in init phase
bool GUI::setUser(string user) {
	int ii;
	for(ii = 0; ii < USERS.size(); ii++) {
		if(user == USERS[ii]) {return 1;}
	}
	USER = user;
	return 0;
}









//read all bans, ensure user isnt already banned, add him to file
void addBan(string user_str) {
	// ofstream banlist;
	// banlist.open("banlist.txt", ios::out | ios::app);
	// if (banlist.is_open()) {
	// 	banlist << user_str + "\n";
	// 	banlist.close();
	// }
	// else {mRecieved("              <ERROR> : Could not open banlist.");}
	//^^ send locally or server wide?
}

//read all bans, ensure user is already banned, remove him, save file.
void remBan(string user_str) {

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
