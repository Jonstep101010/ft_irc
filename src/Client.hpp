#pragma once
#include <string>

#define MAX_NICKNAME_LEN 9 // @todo implement this

class Client {

public:
	Client(std::string const& ip, int socket);
	Client()
		: _ClientSocket()
		, _isServerOperator() {
	} // @audit deprecated, only for testing
	~Client();

	// send a message
	void clientInput(std::string const& message);
	// if currently in a channel, send message to channel (will broadcast to all clients in channel)

	void clientOutput(std::string const& message);

	// _name uniquely identifies a user @follow-up change to map?
	bool operator==(const Client& other) const {
		return this->_name == other._name;
	}

	std::string const& getIP() const { return _ip; }
	int getClientSocket() const { return _ClientSocket; }

	bool getIsConnected() const { return _isConnected; }
	void setIsConnected(bool value) { _isConnected = value; }

	friend class Server;

private:
	std::string _ip;
	int         _ClientSocket;
	std::string _name;
	std::string _nickname;
	bool        _isConnected;
	bool        _isServerOperator;
};
