#include "Server.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <ostream>
#include <poll.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
	: _server_socket(-1), _port(PORT), _server_addr() {}

Server::Server(const Server& src)
	: _server_socket(), _port(PORT), _server_addr() {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server& Server::operator=(Server const& rhs) {
	(void)rhs;
	return *this;
}

/*
@note Function which creates a socket and binds it to the server address
*/
void Server::makeSocket() {
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_socket < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return;
	}
	int opt = 1;
	if (setsockopt(_server_socket, SOL_SOCKET,
				   SO_REUSEADDR | SO_REUSEPORT, &opt,
				   sizeof(opt))
		< 0) {
		std::cerr << "Error setting socket options" << std::endl;
		return;
	}
	_server_addr.sin_family      = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port        = htons(_port);
	if (bind(_server_socket, (struct sockaddr*)(&_server_addr),
			 sizeof(_server_addr))
		< 0) {
		std::cerr << "Error binding socket" << std::endl;
		return;
	}
	if (listen(_server_socket, 3) < 0) {
		std::cerr << "Error listening" << std::endl;
	}

	struct pollfd newPool = {};
	newPool.fd            = _server_socket;
	newPool.revents       = 0;
	newPool.events        = POLLIN;
	_pollfds.push_back(newPool);
}

void Server::acceptConnection(int listeningSocket) {
	sockaddr_in clientAddr;
	socklen_t   clientAddrLen = sizeof(clientAddr);
	int         clientSocket  = accept(
        listeningSocket, (sockaddr*)&clientAddr, &clientAddrLen);
	if (clientSocket == -1) {
		std::cerr << "Accept error: " << strerror(errno)
				  << std::endl;
		return;
	}

	pollfd newPollFd = {clientSocket, POLLIN, 0};
	_pollfds.push_back(newPollFd);

	std::cout << "New connection accepted" << std::endl;
}

void handleClientData(int clientSocket) {
	char    buffer[1024];
	ssize_t bytesReceived
		= recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived == -1) {
		std::cerr << "Recv error: " << strerror(errno)
				  << std::endl;
	} else if (bytesReceived == 0) {
		// Connection closed by client
		std::cout << std::string(buffer, bytesReceived)
				  << std::endl;
		close(clientSocket);
		std::cout << "Client disconnected" << std::endl;
	} else {
		std::cout << "Received data: "
				  << std::string(buffer, bytesReceived);
	}
}
/*
@follow-up Function which starts the server and will wait for connections
@remind The user should be able to choose on which port and ip address to start the server
@note We need to handle signals so when we CTRL + C the server will stop in a clean way
*/
void Server::start() {
	makeSocket();
	std::cout << "Server started on port " << _port << std::endl;
	std::cout << "Waiting for connections..." << std::endl;
	// @follow-up Make the while loop to be until a signal was received
	while (true) {
		// @follow-up Handle poll() and acceptConnection()
		if (poll(_pollfds.data(), _pollfds.size(), 0) == -1) {
			std::cerr << "Poll error: " << strerror(errno)
					  << std::endl;
		}
		for (size_t i = 0; i < _pollfds.size(); ++i) {
			if (_pollfds[i].revents & POLLIN) {
				if (_pollfds[i].fd == _server_socket) {
					std::cout << "We got a new connection"
							  << std::endl;
					acceptConnection(_pollfds[i].fd);
				} else {
					handleClientData(_pollfds[i].fd);
					// _pollfds[0].revents = 0;
				}
			}
		}
		// Close file descriptors
	}
}