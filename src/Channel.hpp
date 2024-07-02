#pragma once
#include "Client.hpp"
#include <iostream>
#include <string>
#include <vector>

class Client;

class Channel {

public:
	Channel();
	Channel(Channel const& src);
	~Channel();

	Channel& operator=(Channel const& rhs);

	/// USER MANAGEMENT ///
	// add user to channel
	void addUser(Client const& client);
	// remove user from channel
	void removeUser(Client const& client);

	// send message to all Clients in channel
	void Message(std::string const& message);

	/// CHANNEL MANAGEMENT ///
	// set channel topic
	void setTopic(std::string const& newtopic);
	// set mode method (will be directed to member functions)
	void setMode(std::string const& mode);
	// @todo add mode specific functions

private:
	const std::string   _name;
	std::string         _topic;
	std::vector<Client> _clients;
	std::vector<bool>   _is_operator;

	bool        _is_invite_only;
	bool        _topic_protection;
	std::string _key; // if empty, no key required
};
