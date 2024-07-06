#include "Channel.hpp"
#include "Server.hpp"
#include "defines.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iostream>
#include <ostream>
#include <poll.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

std::string get_cmd(std::string data) {
	size_t pos = data.find_first_of(" ");
	if (pos != std::string::npos) {
		return std::string(data.std::string::substr(0, pos));
	}
	return "";
}

std::string get_after_cmd(std::string data) {
	size_t pos = data.find_first_of(" ");
	if (pos != std::string::npos) {
		return std::string(data.std::string::substr(pos + 1));
	}
	return "";
}

void Server::join(std::string   channel_name,
				  Client const& client) {
	if (find_cnl(channel_name, _channels) == _channels.end()) {
		_channels.push_back(Channel(channel_name));
	}
	find_cnl(channel_name, _channels)->addUser(client);
}

void Server::privmsg(std::string after, Client const& client) {
	std::string dest = after.substr(0, after.find_first_of(" "));
	std::string message
		= after.substr(after.find_first_of(" ") + 2);
	std::vector<Channel>::iterator dest_channel
		= Server::find_cnl(dest, _channels);
	if (dest_channel != _channels.end()) {
		dest_channel->Message(client, PRIVMSG_CHANNEL);
	} else {
		std::vector<Client>::iterator dest_client
			= Server::findnick(dest, _clients);
		if (dest_client != _clients.end()) {
			dest_client->Output(PRIVMSG_USER);
		}
	}
}

void Server::quit(std::string after, Client const& client) {
	std::cout << "Client quit";
	after.empty() ? std::cout << std::endl
				  : std::cout << ": " << after << std::endl;
	// remove user from all channels
	for (std::vector<Channel>::iterator it = _channels.begin();
		 it != _channels.end(); ++it) {
		it->removeUser(client);
	}
	// remove user from clients
	close(client._ClientSocket);
	_clients.erase(
		std::find(_clients.begin(), _clients.end(), client));
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd == client._ClientSocket) {
			_pollfds.erase(_pollfds.begin() + i);
		}
	}
}

void Server::part(std::string after, Client const& client) {
	// Extract channel name
	std::string channel_name
		= after.substr(0, after.find_first_of(" "));

	// Find the channel
	std::vector<Channel>::iterator it
		= find_cnl(channel_name, _channels);
	if (it != _channels.end()) {

		// Check if client is part of the channel
		if (std::find(it->_clients.begin(), it->_clients.end(),
					  client)
			!= it->_clients.end()) {
			it->Message(client, PART_REPLY(client, it->_name));
			it->removeUser(client);
		} else {
			std::cout << "Client " << client._nickname
					  << " is not part of channel "
					  << channel_name << std::endl;
		}
	} else {
		std::cout << "Channel " << channel_name << " not found"
				  << std::endl;
	}
}

void Server::executeCommand(Client const&      client,
							std::string const& data) {
	std::string cmd   = get_cmd(data);
	std::string after = get_after_cmd(data);
	if (data == "QUIT" || cmd == "QUIT") {
		// message needs to be broadcastest to the whole channel that he is in.
		// when no message, then just display <name> client quits to the channel
		// @todo output to channels
		quit(after, client);
	}
	if (cmd == "PART") {
		part(after, client);
	}
	if (!after.empty()) {
		if (cmd == "JOIN" && after[0] == '#') {
			join(after.substr(0, after.find_first_of("\r\n")),
				 client);
		}
		if (cmd == "PRIVMSG") {
			if (!after.empty()) {
				privmsg(after, client);
			}
		}
	}
}