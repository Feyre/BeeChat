/* GUI.h
 * ENB241 Project
 *
 * GUI class header
 *
 */

#ifndef _GUI_H
#define _GUI_H
#include <string>
#include <vector>
#include <queue>

#include <ncurses.h>
using std::string;
using std::vector;
using std::queue;

#include "User.h"


class GUI
{
public:
	GUI(User user);
	~GUI();
	void initScreen();
	void initWindows();
	void Routine();
	void resize();

	string mRequested();
	void mRecieved(string message);

	void updateUsers(vector<User> users);
	void removeUser(string user);

	int checkVulgar(string *message);
	string upperCase(string message);

	
// private:
	void printAscii(WINDOW *scr, char *gaphics[], int sizey, int sizex, int starty, int startx);
	void printServers(WINDOW *scr, int a);
	void printMessage(string message, int message_lines, int *h_index, int *attempt, int *message_len, int prev_len, int endline);
	void printUsers(WINDOW *scr);
	void printTimeout();
	void printChat();
	
	void userInput();
	void userInputB();
	int selectServer();
	void showScreen(int a, int b);
	bool command(string message, int out_in);

	void test();

private:
	User _user;

	vector<User> _users;
	vector<string> DATA;
	queue<string> MESSAGE_QUEUE;

	WINDOW *info_scr;
	WINDOW *servers_scr;
	WINDOW *chat_win;
	WINDOW *message_win;
	WINDOW *servers_win;
};



#endif
