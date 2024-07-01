#pragma once
#include "Channel.hpp"
#include <string>
#include <vector>

class Channel;

class User {

public:
	User();
	User(User const& src);
	~User();

	// need to make sure username is unique @follow-up
	User(std::string const& username);
	User& operator=(User const& rhs);

	// send a message to the user
	void sendMessage(std::string const& message);

	// join a channel
	void joinChannel(std::string const& channel);

	// leave a channel
	void leaveChannel(std::string const& channel);

	// _username uniquely identifies a user @follow-up change to map?
	bool operator==(const User& other) const {
		return this->_username == other._username;
	}

private:
	std::string          _username;
	std::string          _nickname;
	bool                 _isOperator;
	std::vector<Channel> _channels;
};
