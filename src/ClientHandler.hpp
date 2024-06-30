#pragma once
#include "User.hpp"
#include <iostream>
#include <string>

class ClientHandler {

public:
	ClientHandler();
	ClientHandler(ClientHandler const& src);
	~ClientHandler();

	ClientHandler& operator=(ClientHandler const& rhs);

	// do we need the main loop in here (for client in/out)? @follow-up

	void authenticate();

	// should parse and execute? @follow-up
	void parseCommand(std::string const& command);

private:
	int  _ClientSocket;
	User _connected_user;
};
