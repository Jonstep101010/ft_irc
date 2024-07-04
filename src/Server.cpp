#include "Server.hpp"
#include <arpa/inet.h>
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
	: _running(true)
	, _server_socket(-1)
	, _port(PORT)
	, _server_addr() {
	instance = this;
}

Server::Server(const Server& src)
	: _running(), _server_socket(), _port(PORT), _server_addr() {
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server() {
	if (_server_socket >= 0) {
		close(_server_socket);
	}
}

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

	// Make a client and add it to the vector
	std::string ip = inet_ntoa(clientAddr.sin_addr);
	Client      newClient(ip, clientSocket);
	_clients.push_back(newClient);

	std::cout << "New connection accepted" << std::endl;
}

void Server::handleClientData(Client& client) {
	char    buffer[1024];
	ssize_t bytesReceived
		= recv(client._ClientSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived == -1) {
		std::cerr << "Recv error: " << strerror(errno)
				  << std::endl;
	} else if (bytesReceived == 0) {
		// @follow-up NOTE this we still dont know when happens
	} else {
		std::cout << "Received data: "
				  << std::string(buffer, bytesReceived);
		if (!client._isConnected) {
			/* @note Have to parse the data and set Nickname and Username if the user is first connected to the server 
			* @todo After you parse and set the nickname and username into the Client class you have to send a welcome message to the client
			*/

			/* @follow-up This are just examples of how to send data to the client
			std::string welcomeMsg
				= ":localhost 001 Aceauses :Welcome "
				  "to the IRC server!\r\n";
			client.setIsConnected(true);
			send(client.getClientSocket(), welcomeMsg.c_str(),
				 welcomeMsg.length(), 0);*/
			// if (std::string(buffer, bytesReceived).find("NICK")
			// 	!= std::string::npos) {
			// 	client.setNickname(
			// 		std::string(buffer, bytesReceived)
			// 			.substr(5));
			// }
			// SHOULD BE CHANGED?
			char* line = strtok(buffer, "\n");

			while (line != NULL) {
				if (line[strlen(line) - 1] == '\r') {
					line[strlen(line) - 1] = '\0';
				}
				if (strncmp(line, "NICK ", 5) == 0) {
					char* nick       = line + 5;
					client._nickname = nick;
					std::cout
						<< "[DEBUG] Nickname set to: " << nick
						<< std::endl;
				}
				if (strncmp(line, "USER ", 5) == 0) {
					char* user          = line + 5;
					client._name        = user;
					client._isConnected = true;
					std::cout
						<< "[DEBUG] Username set to: " << user
						<< std::endl;
				}
				line = strtok(NULL, "\n");
			}
			client._isConnected      = true;
			client._isServerOperator = false;
		}
		// std::string joinCnl
		// 	= ":Aceauses@localhost JOIN #test\r\n";
		// send(client.getClientSocket(), joinCnl.c_str(),
		// 	 joinCnl.size(), 0);
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
	while (_running) {
		// @follow-up Handle poll() and acceptConnection()
		if (poll(_pollfds.data(), _pollfds.size(), 0) == -1
			&& errno != EINTR) {
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
					handleClientData(_clients[i - 1]);
					// _pollfds[0].revents = 0;
					std::cout
						<< "username: " << _clients[i - 1]._name
						<< std::endl;
					std::cout << "nickname: "
							  << _clients[i - 1]._nickname
							  << std::endl;
				}
			}
		}
		// Close file descriptors
	}
}