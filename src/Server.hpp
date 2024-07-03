#pragma once
#include "Channel.hpp"
#include "Client.hpp"
#include <netinet/in.h>
#include <vector>

class Client;
class Channel;

class Server {
public:
	Server();
	Server(Server const& src);
	~Server();

	Server& operator=(Server const& rhs);

	void makeSocket();
	void start();
	void acceptConnection(int listeningSocket);

	// manage commands passed by clients @follow-up
	void executeCommand(std::string const& command);

	// join a channel
	void joinChannel(Client const&  client,
					 Channel const& channel);

	// leave a channel
	void leaveChannel(Client const&  client,
					  Channel const& channel);

	// signal and free
	static void staticWrapperSignal(int sig);
	void        signalHandler(int sig);

	void handleClientData(Client client);

private:
	static Server*             instance;
	bool                       _running;
	int                        _server_socket;
	int                        _port;
	struct sockaddr_in         _server_addr; // Not required here
	std::vector<Client>        _clients;
	std::vector<Channel>       _channels;
	std::vector<struct pollfd> _pollfds;

	// for finding a channel or client by name, inside any class Server can access
	template <typename Container>
	typename Container::iterator
	findname(std::string const& instance_name,
			 Container&         collection) {

		for (typename Container::iterator it
			 = collection.begin();
			 it != collection.end(); ++it) {
			if (it->_name == instance_name) {
				return it;
			}
		}
		return collection.end();
	}
};
