#include "User.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

User::User()
	: _isOperator(false) {}

User::User(const User& src)
	: _isOperator() {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

User::~User() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

User& User::operator=(User const& rhs) {
	if (this != &rhs) {
		_username   = rhs._username;
		_nickname   = rhs._nickname;
		_isOperator = rhs._isOperator;
		_channels   = rhs._channels;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// send a message to the user
void User::sendMessage(std::string const& message) {
	(void)message;
	// send message to user
	// how to? @follow-up
}

// @follow-up the channel has to exist on the server
// join a channel
void User::joinChannel(std::string const& channel) {
	(void)channel;
	// find channel
	// if found, get the channel and call its addUser(&this)
}

// leave a channel
// @follow-up the channel has to exist on the server
void User::leaveChannel(std::string const& channel) {
	(void)channel;
	// find channel
	// if found, get the channel and call its removeUser(&this)
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */