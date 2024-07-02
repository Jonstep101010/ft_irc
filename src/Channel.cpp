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
		_clients = rhs._clients;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// add user to channel
void Channel::addUser(Client const& client) {
	_clients.push_back(client);
}

// remove user from channel
void Channel::removeUser(Client const& client) {
	// find user
	// if found, remove user from channel
	std::vector<Client>::iterator toRemove
		= std::find(_clients.begin(), _clients.end(), client);

	if (toRemove != _clients.end()) {
		_clients.erase(toRemove);
	}
}

// send message to all users in channel
void Channel::Message(std::string const& message) {
	(void)message;
	// send message to all users in channel
	// how to? @follow-up
}

// set channel topic
void Channel::setTopic(std::string const& newtopic) {
	_topic = newtopic;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */