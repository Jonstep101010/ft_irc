#include "Client.hpp"
#include <sys/socket.h>

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