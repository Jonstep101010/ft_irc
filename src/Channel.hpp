#pragma once
#include "Client.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Client;

class Channel {

public:
	/*
	** ------------------------------- CONSTRUCTOR --------------------------------
	*/
	Channel(std::string const& name)
		: _name(name), _is_invite_only(), _topic_protection() {}

	Channel()
		: _is_invite_only(), _topic_protection() {}

	Channel(const Channel& src)
		: _name(src._name)
		, _topic(src._topic)
		, _clients(src._clients)
		, _is_operator(src._is_operator)
		, _is_invite_only(src._is_invite_only)
		, _topic_protection(src._topic_protection)
		, _key(src._key) {}
	~Channel() {}

	bool operator==(const Client& other) const {
		return this->_name == other._name;
	}

	bool is_operator(Client const& client) {
		std::vector<Client>::iterator it = std::find(
			_clients.begin(), _clients.end(), client);
		if (it != _clients.end()) {
			return _is_operator[it - _clients.begin()];
		}
		return false;
	}

	// @follow-up add method to get a mutable reference to a Client operator status

	/// USER MANAGEMENT ///
	// add user to channel
	void addUser(Client const& client);
	// remove user from channel
	void removeUser(Client const& client);

	// send message to all Clients in channel
	void Message(Client const&      origin,
				 std::string const& message);

	/// CHANNEL MANAGEMENT ///
	// set channel topic
	void setTopic(std::string& new_topic);
	// set mode method (will be directed to member functions)
	void setMode(std::string const& mode);
	// @todo add mode specific functions

	friend class Server;

private:
	const std::string   _name;
	std::string         _topic;
	std::vector<Client> _clients;
	std::vector<bool>   _is_operator;

	bool        _is_invite_only;
	bool        _topic_protection;
	std::string _key; // if empty, no key required
};
