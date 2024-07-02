#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(std::string const& ip, int socket,
			   std::string const& _username,
			   std::string const& _nickname)
	: _ip(ip)
	, _ClientSocket(socket)
	, _username(_username)
	, _nickname(_nickname)
	, _isServerOperator(false) {
	(void)_isServerOperator;
}

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
void Client::clientOutput(std::string const& message) {
	(void)message;
}

// client sends a message
void Client::clientInput(std::string const& message) {
	(void)message;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */