#include "Client.hpp"
#include <sys/socket.h>

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

// send a message to the Client
void Client::Output(std::string const& message) const {
	send(_ClientSocket, message.c_str(), message.size(), 0);
}

// client sends a message
void Client::Input(std::string const& message) {
	(void)message;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */