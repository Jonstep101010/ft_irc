#include "Channel.hpp"
#include "Server.hpp"
#include "debug.hpp"
#include "defines.hpp"
#include "typedef.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

// add user to channel
// @follow-up handle modes, Client already joined, limit reached
void Channel::addUser(const Client& client) {
	ClientOpIt it
		= std::find_if(_clients_op.begin(), _clients_op.end(),
					   CompareClient(client));

	if (it == _clients_op.end()) {
		client.Output(JOINEDREPLY);
		_clients_op.push_back(std::make_pair(client, false));
		Channel::Message(client, JOINED_NOTICE);
	}
}

// remove user from channel
void Channel::removeUser(Client const& client) {
	// find user
	// if found, remove user from channel
	ClientOpIt toRemove
		= std::find_if(_clients_op.begin(), _clients_op.end(),
					   CompareClient(client));

	if (toRemove != _clients_op.end()) {
		_clients_op.erase(toRemove);
		// make everyone operator if last one left
		if (std::find_if(_clients_op.begin(), _clients_op.end(),
						 CompareOperator(true))
			== _clients_op.end()) {
			for (ClientOpIt it = _clients_op.begin();
				 it != _clients_op.end(); it++) {
				it->second = true;
			}
		}
	}
}

// send message to all users in channel
void Channel::Message(Client const&      origin,
					  std::string const& message) {
	// send message to all users in channel
	// how to? @follow-up
	if (_clients_op.empty()) { return; }
	std::ostringstream users;
	for (size_t i = 0; i < _clients_op.size(); i++) {
		users << _clients_op[i].first._nickname << ", ";
	}
	debug(CHANNEL, "Channel name: [" + this->_name + "] Users: ["
					   + users.str() + "]");
	if (findnick(origin._nickname) == _clients_op.end()) {
		return;
	}
	for (ClientOpIt it = _clients_op.begin();
		 it != _clients_op.end(); it++) {
		if (it->first == origin) {
			debug(DEBUG, "Didn't send to " + it->first._nickname
							 + " Cause he is origin");
			continue;
		}
		it->first.Output(message);
	}
}

// set channel topic
void Channel::setTopic(std::string& new_topic) {
	new_topic[0] == ' ' ? new_topic = new_topic.substr(1)
						: new_topic;
	new_topic[0] == ':' ? new_topic = new_topic.substr(1)
						: new_topic;
	_topic = new_topic;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */