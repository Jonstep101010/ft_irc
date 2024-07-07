#include "Channel.hpp"
#include "Server.hpp"
#include "Utils.hpp"
#include "debug.hpp"
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

void Server::join(std::string   channel_name,
				  Client const& client) {
	std::vector<Channel>::iterator to_join
		= find_cnl(channel_name, _channels);
	if (to_join == _channels.end()) {
		Channel new_cnl(channel_name);
		new_cnl.addUser(client);
		new_cnl._is_operator[0] = true;
		_channels.push_back(new_cnl);
	} else {
		to_join->addUser(client);
	}
}

void Server::privmsg(std::string after, Client const& client) {
	std::string dest = after.substr(0, after.find_first_of(" "));
	std::string message
		= after.substr(after.find_first_of(" ") + 2);
	if (dest[0] == '#' || dest[0] == '&') {
		std::vector<Channel>::iterator dest_channel
			= Server::find_cnl(dest, _channels);
		if (dest_channel != _channels.end()) {
			dest_channel->Message(client, PRIVMSG_CHANNEL);
		}
	} else {
		std::vector<Client>::iterator dest_client
			= Server::findnick(dest, _clients);
		if (dest_client != _clients.end()) {
			dest_client->Output(PRIVMSG_USER);
		}
	}
}

void Server::quit(std::string after, Client const& client) {
	debug(CLIENT, "Client Quit [" + client._nickname + "]");
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

void Server::topic(std::string after, Client const& client) {
	std::string channel_name = get_cnl(after);
	std::string new_topic
		= get_additional(after); // @todo skip space and colon
	if (!channel_name.empty()) {
		std::vector<Channel>::iterator channel
			= find_cnl(channel_name, _channels);
		if (channel != _channels.end()) {
			if (new_topic.empty()) {
				// only single channel is given as argument
				if (channel != _channels.end()) {
					if (channel->_topic.empty()) {
						client.Output(RPL_NOTOPIC);
					} else {
						client.Output(RPL_TOPIC);
					}
				}
			} else {
				// @todo check if user is operator of the channel
				// if so, allow it to change the topic
				if (!channel->_topic_protection) {
					// allow anyone to change the topic
					channel->setTopic(new_topic);
				} else {
					// @follow-up simplify this
					std::vector<Client>::iterator client_it
						= std::find(channel->_clients.begin(),
									channel->_clients.end(),
									client);
					if (client_it != channel->_clients.end()) {
						long client_idx
							= distance(channel->_clients.begin(),
									   client_it);
						if (channel->_is_operator[client_idx]) {
							channel->setTopic(new_topic);
						} else {
							client.Output(ERR_CHANOPRIVSNEEDED);
						}
					}
				}
			}
		}
	}
}

void Server::executeCommand(Client const&      client,
							std::string const& data) {
	std::string cmd   = get_cmd(data);
	std::string after = get_after_cmd(data);
	if (data == "QUIT" || cmd == "QUIT") {
		quit(after, client);
	} else if (!after.empty()) {
		if (cmd == "JOIN"
			&& (after[0] == '#' || after[0] == '&')) {
			join(after, client);
		} else if (cmd == "PRIVMSG") {
			privmsg(after, client);
		} else if (cmd == "TOPIC"
				   && (after[0] == '#' || after[0] == '&')) {
			topic(after.substr(0, after.find_first_of("\r\n")),
				  client);
			// @follow-up do this in get_after_cmd?
		}
	}
}