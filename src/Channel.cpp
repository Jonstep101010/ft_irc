#include "Channel.hpp"
#include "Server.hpp"
#include "debug.hpp"
#include "defines.hpp"
#include "typedef.hpp"
#include <algorithm>
#include <sstream>
#include <vector>

void Channel::addUser(const Client& client) {
	ClientOpIt it
		= std::find_if(_clients_op.begin(), _clients_op.end(),
					   CompareClient(client));

	if (static_cast<int>(_clients_op.size()) >= _limit
		&& _limit != -1) {
		throw LimitReached();
	}
	if (it == _clients_op.end()) {
		client.Output(JOINEDREPLY);
		_clients_op.push_back(std::make_pair(client, false));
		Channel::Message(client, JOINED_NOTICE);
	}
}

void Channel::removeUser(Client const& client) {
	ClientOpIt toRemove
		= std::find_if(_clients_op.begin(), _clients_op.end(),
					   CompareClient(client));

	if (toRemove != _clients_op.end()) {
		_clients_op.erase(toRemove);
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

void Channel::Message(Client const&      origin,
					  std::string const& message) {
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

void Channel::setTopic(std::string& new_topic) {
	new_topic[0] == ' ' ? new_topic = new_topic.substr(1)
						: new_topic;
	new_topic[0] == ':' ? new_topic = new_topic.substr(1)
						: new_topic;
	_topic = new_topic;
}