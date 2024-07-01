#pragma once
#include "User.hpp"
#include <iostream>
#include <string>
#include <vector>

class User;

class Channel {

public:
	Channel();
	Channel(Channel const& src);
	~Channel();

	Channel& operator=(Channel const& rhs);

	std::string name;// can names be changed? @follow-up
	std::string topic;

	// add user to channel
	void addUser(User const& user);

	// remove user from channel
	void removeUser(User const& user);

	// send message to all users in channel
	void broadcastMessage(std::string const& message);

	// set channel topic
	void setTopic(std::string const& newtopic);

	// set mode method
	// @follow-up

private:
	std::vector<User> _users;
	// operator list? @follow-up
	// mode settings? @follow-up
};
