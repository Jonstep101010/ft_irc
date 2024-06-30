#pragma once
#include <iostream>
#include <string>

class ClientHandler {

public:
	ClientHandler();
	ClientHandler(ClientHandler const& src);
	~ClientHandler();

	ClientHandler& operator=(ClientHandler const& rhs);

private:
};
