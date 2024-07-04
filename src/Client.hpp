#pragma once
#include <string>

#define MAX_NICKNAME_LEN 9 // @todo implement this

class Client {

public:
	Client(std::string const& ip, int socket);
	Client(const std::string& src)
		: _ClientSocket(), _isConnected(), _isServerOperator() {
		*this = src;
	}
	Client& operator=(const std::string& src) {
		(void)src;
		return *this;
	}
	Client()
		: _ClientSocket()
		, _isConnected()
		, _isServerOperator() {
	} // @audit deprecated, only for testing
	~Client();

	// send a message
	void clientInput(std::string const& message);
	// if currently in a channel, send message to channel (will broadcast to all clients in channel)

	void clientOutput(std::string const& message) const;

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
