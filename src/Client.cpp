#include "Client.hpp"
#include "defines.hpp"
#include <sys/socket.h>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(std::string const& ip, int socket)
	: _ip(ip)
	, _ClientSocket(socket)
	, _name(USER_DEFAULT_NAME)
	, _nickname(USER_DEFAULT_NAME)
	, _isConnected()
	, _isServerOperator(false) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

// send a message to the Client
void Client::clientOutput(std::string const& message) const {
	(void)message;
	send(_ClientSocket, message.c_str(), message.size(), 0);
}

// client sends a message
void Client::clientInput(std::string const& message) {
	(void)message;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */