#include "Channel.hpp"
#include "Server.hpp"
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
		Channel::Message(client, JOINED_NOTICE);
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
void Channel::Message(Client const&      origin,
					  std::string const& message) {
	// send message to all users in channel
	// how to? @follow-up
	for (size_t i = 0; i < _clients.size(); i++) {
		std::cout << "[CHANNEL USERS] " << _clients[i]._nickname
				  << std::endl;
	}
	if (_clients.empty()
		|| Server::findnick(origin._nickname, _clients)
			   == _clients.end()) {
		return;
	}
	for (std::vector<Client>::iterator it = _clients.begin();
		 it != _clients.end(); it++) {
		if (*it == origin) {
			std::cout << "[SKIP] " << it->_name << std::endl;
			continue;
		}
		it->Output(message);
	}
}

// set channel topic
void Channel::setTopic(std::string const& newtopic) {
	_topic = newtopic;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */