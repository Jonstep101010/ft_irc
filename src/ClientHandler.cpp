#include "ClientHandler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientHandler::ClientHandler() {}

ClientHandler::ClientHandler(const ClientHandler& src) {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ClientHandler::~ClientHandler() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ClientHandler&
ClientHandler::operator=(ClientHandler const& rhs) {
	//if ( this != &rhs )
	//{
	//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */