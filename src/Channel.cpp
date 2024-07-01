#include "Channel.hpp"
#include <algorithm>
#include <iterator>
#include <vector>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel() {}

Channel::Channel(const Channel& src) { *this = src; }

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel& Channel::operator=(Channel const& rhs) {
	if (this != &rhs) {
		_users = rhs._users;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// add user to channel
void Channel::addUser(User const& user) { _users.push_back(user); }

// remove user from channel
void Channel::removeUser(User const& user) {
	// find user
	// if found, remove user from channel
	std::vector<User>::iterator toRemove
		= std::find(_users.begin(), _users.end(), user);

	if (toRemove != _users.end()) {
		_users.erase(toRemove);
	}
}

// send message to all users in channel
void Channel::broadcastMessage(std::string const& message) {
	(void)message;
	// send message to all users in channel
	// how to? @follow-up
}

// set channel topic
void Channel::setTopic(std::string const& newtopic) {
	topic = newtopic;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */