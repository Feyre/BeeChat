/* User.cpp
 * ENB241 Project
 *
 * User class implementation
 *
 */

#include "User.h"

User::User(string username, int priviledges) {
	// ensure other classes make sure the username isnt already used
	_swearjar = 0;
	_username = username;
	_priviledges = priviledges;
}

User::User() {

}

User::~User() {

}

string User::getUser() {
	return _username;
}

int User::getSwears() {
	return _swearjar;
}

void User::incSwears() {
	_swearjar++;
}


int User::getPriviledges() {
	return _priviledges;
}

void User::setPriviledges(int type) {
	// if(power > ADMIN) {
		_priviledges = type;
	// 	return 1;
	// }
	// return 0;
}
