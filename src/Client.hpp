#pragma once
#include "defines.hpp"
#include <string>

#define MAX_NICKNAME_LEN 9 // @todo implement this

class Client {

public:
	/*
	** ------------------------------- CONSTRUCTOR --------------------------------
	*/
	Client(std::string const& ip, int socket)
		: _ip(ip)
		, _ClientSocket(socket)
		, _isConnected()
		, _isServerOperator(false) {}
	~Client() {}

	// send a message
	void Input(std::string const& message);
	// if currently in a channel, send message to channel (will broadcast to all clients in channel)

	void Output(std::string const& message) const;

	// _name uniquely identifies a user @follow-up change to map?
	bool operator==(const Client& other) const {
		return this->_name == other._name;
	}

	friend class Server;
	friend class Channel;

private:
	std::string _ip;
	int         _ClientSocket;
	std::string _name;
	std::string _nickname;
	bool        _isConnected;
	bool        _isServerOperator;
};
