#pragma once
#include <string>

#define MAX_NICKNAME_LEN 9 // @todo implement this

class Client {

public:
	Client(std::string const& ip, int socket,
		   std::string const& _username,
		   std::string const& _nickname);
	Client()
		: _ClientSocket()
		, _isServerOperator() {
	} // @audit deprecated, only for testing
	~Client();

	// send a message
	void clientInput(std::string const& message);
	// if currently in a channel, send message to channel (will broadcast to all clients in channel)

	void clientOutput(std::string const& message);

	// _username uniquely identifies a user @follow-up change to map?
	bool operator==(const Client& other) const {
		return this->_username == other._username;
	}

	std::string const& getIP() const { return _ip; }
	int getClientSocket() const { return _ClientSocket; }

private:
	std::string _ip;
	int         _ClientSocket;
	std::string _username;
	std::string _nickname;
	bool        _isServerOperator;
};
