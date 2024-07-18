#pragma once
#include "debug.hpp"
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>

#define MAX_NICKNAME_LEN 9

class Client {

public:
	Client(std::string const& ip, int socket)
		: _ip(ip)
		, _ClientSocket(socket)
		, _isConnected()
		, _LoggedIn(false)
		, _last_ping_sent(time(NULL))
		, _awaiting_pong(false) {}
	~Client() {}

	void Output(std::string const& message) const {
		std::ostringstream socketInfo;
		socketInfo << "Number: " << _ClientSocket
				   << " With Name: " << _nickname;
		debug(SOCKET, socketInfo.str());
		debug(MESSAGE_OUTPUT, message);
		send(_ClientSocket, message.c_str(), message.size(), 0);
	}

	bool operator==(const Client& other) const {
		return this->_nickname == other._nickname;
	}

	friend class Server;
	friend class Channel;

private:
	std::string _inputBuffer;
	std::string _ip;
	int         _ClientSocket;
	std::string _name;
	std::string _nickname;
	bool        _isConnected;
	bool        _LoggedIn;
	time_t      _last_ping_sent;
	bool        _awaiting_pong;
};