#pragma once
#include "User.hpp"
#include <iostream>
#include <string>

class User;

class Channel {

public:
	Channel();
	Channel(Channel const& src);
	~Channel();

	Channel& operator=(Channel const& rhs);

	std::string name;
	std::string topic;

	// add user to channel
	void addUser(User const& user);

	// remove user from channel
	void removeUser(User const& user);

	// send message to all users in channel
	void broadcastMessage(std::string const& message);

	// set channel topic
	void setTopic(std::string const& topic);

	// set mode method
	// @follow-up

private:
	User* _users; // use vector? @follow-up
				  // operator list? @follow-up
				  // mode settings? @follow-up
};
