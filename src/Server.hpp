#pragma once
#include "Bot.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "typedef.hpp"
#include <netinet/in.h>
#include <string>
#include <vector>

class Client;
class Channel;

class Server {
public:
	Server();
	Server(int port, std::string password);
	Server(Server const& src);
	~Server();

	Server& operator=(Server const& rhs);

	void makeSocket();
	void start();
	void acceptConnection(int listeningSocket);

	// manage commands passed by clients @follow-up
	void executeCommand(Client& client, std::string const& data);

	// join a channel
	void join(std::string channel_name, Client const& client);

	// send message to channel or user
	void privmsg(std::string after_cmd, Client const& client);

	// quit the connection
	void quit(std::string after_cmd, Client const& client);

	// topic (set if operator (args), get if not (no args))
	void topic(std::string after_cmd, Client const& client);

	// removes client from channel
	void part(std::string after, Client const& client);

	// kick client from channel
	void kick(std::string after, Client const& client);

	// mode message (change multiple settings)
	void mode(std::string after_cmd, Client const& client);

	// invite a user to a channel
	void invite(std::string after_cmd, Client const& client);

	// change own nickname on server
	void nick(std::string after_cmd, Client& client);

	// signal and free
	static void staticWrapperSignal(int sig);
	void        signalHandler(int sig);

	void handleClientData(Client& client);
	bool checkIfAlreadyConnected(Client& client);
	void handleInitialConnection(Client&            client,
								 const std::string& message);
	void handlePassCommand(Client& client, std::string password);
	void processClientBuffer(Client& client);

	void pingClients();

	friend void Channel::Message(Client const&      origin,
								 std::string const& message);

	static std::vector<Channel>::iterator
	find_cnl(std::string const&    channel,
			 std::vector<Channel>& channels) {

		for (std::vector<Channel>::iterator it
			 = channels.begin();
			 it != channels.end(); ++it) {
			if (it->_name == channel) { return it; }
		}
		return channels.end();
	}

	static std::vector<Client>::iterator
	findnick(std::string const&   instance_name,
			 std::vector<Client>& collection) {

		for (std::vector<Client>::iterator it
			 = collection.begin();
			 it != collection.end(); ++it) {
			if (it->_nickname == instance_name) { return it; }
		}
		return collection.end();
	}

private:
	static Server*             instance;
	bool                       _running;
	int                        _server_socket;
	int                        _port;
	std::string                _server_ip;
	std::string                _server_pass;
	std::vector<Client>        _clients;
	std::vector<Channel>       _channels;
	std::vector<struct pollfd> _pollfds;
	Bot                        _server_bot;
};
