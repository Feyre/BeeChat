/* User.h
 * ENB241 Project
 *
 * User class header
 *
 */

#ifndef _USER_H
#define _USER_H

#include <string>
using std::string;

#define TIMEDOUT   -1
#define SPECTATOR	0
#define REGULAR		1
#define ADMIN		2
#define SADMIN		3

class User
{
public:
	User(string username, int priviledges);
	User();
	~User();

	string getUser();
	int getSwears();
	void incSwears();
	int getPriviledges();
	void setPriviledges(int type);

private:
	int _swearjar;
	int _priviledges;
	string _username;
};



#endif
