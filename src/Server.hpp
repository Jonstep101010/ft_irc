#pragma once
#include "ClientHandler.hpp"
#include <netinet/in.h>
#include <vector>

class ClientHandler;

class Server {
public:
	Server();
	Server(Server const& src);
	~Server();

	Server& operator=(Server const& rhs);

	void makeSocket();
	void start();

private:
	int                        _server_socket;
	int                        _port;
	struct sockaddr_in         _server_addr; // Not required here
	std::vector<ClientHandler> _clients;
	std::vector<struct pollfd> _pollfds;
};
