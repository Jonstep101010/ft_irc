#include "Channel.hpp"
#include "defines.hpp"
#include <algorithm>
#include <iterator>
#include <vector>

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

// add user to channel
// @follow-up handle modes, Client already joined, limit reached
void Channel::addUser(Client const& client) {
	if (std::find(_clients.begin(), _clients.end(), client)
		== _clients.end()) {
		client.Output(JOINEDREPLY);
		_clients.push_back(client);
	}
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