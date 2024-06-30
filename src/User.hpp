#pragma once
#include <iostream>
#include <string>

class User {

public:
	User();
	User(User const& src);
	~User();

	User& operator=(User const& rhs);

	// send a message to the user
	void sendMessage(std::string const& message);

	// join a channel
	void joinChannel(std::string const& channel);

	// leave a channel
	void leaveChannel(std::string const& channel);

private:
	std::string _username;
	std::string _nickname;
	// operator status? @follow-up
	// list of channels? @follow-up
};
