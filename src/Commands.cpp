#include "Channel.hpp"
#include "Server.hpp"
#include "Utils.hpp"
#include "debug.hpp"
#include "defines.hpp"
#include "typedef.hpp"
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
		new_cnl._clients_op[0].second = true;
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
	const std::string              channel_name = get_cnl(after);
	std::vector<Channel>::iterator channel
		= find_cnl(channel_name, _channels);
	if (channel == _channels.end()) { return; }
	std::string new_topic = get_additional(after);
	if (new_topic.empty()
		&& after[after.find_first_of(" ") + 1] != ':') {
		// only single channel is given as argument, possible postfix of space
		channel->_topic.empty() ? client.Output(RPL_NOTOPIC)
								: client.Output(RPL_TOPIC);
	} else {
		!channel->_topic_protection
				|| channel->is_operator(client)
			? channel->setTopic(new_topic)
			: client.Output(ERR_CHANOPRIVSNEEDED);
	}
}

typedef enum e_modes {
	INV_ONLY      = 'i',
	KEY_SET       = 'k',
	OP_PERM       = 'o',
	TOPIC_PROTECT = 't',
} MODES;

typedef struct t_setmode {
	MODES   to_mod;
	MODE_OP op_todo;
} MODE_SET;

std::string get_additional_mode(std::string data) {
	size_t pos = data.find_first_of(" ");
	if (pos != 0 && pos != std::string::npos) {
		pos = data.find_first_not_of(" ", pos);
		if (pos != std::string::npos) {
			return data.substr(pos);
		}
	}
	return "";
}

void Server::mode(std::string after, Client const& client) {
	const std::string channel_name = get_cnl(after);
	std::string       after_cnl    = get_additional_mode(after);
	// @note handle error
	if (after_cnl.empty()) { return; }
	std::vector<Channel>::iterator channel
		= find_cnl(channel_name, _channels);
	// @todo handle error, channel not existing, user not being member,...
	if (!channel->is_operator(client)) { return; }
	// @note priviledged access
	MODE_SET change_req;
	// @follow-up more sophisticated parsing/checking
	// @todo handle error
	if ((after_cnl[0] != ADD && after_cnl[0] != RM)
		|| !strchr("ikot", after_cnl[1])
		|| after_cnl[2] != ' ') {
		return;
	}
	change_req.op_todo
		= static_cast<MODE_OP>(static_cast<int>(after_cnl[0]));
	change_req.to_mod
		= static_cast<MODES>(static_cast<int>(after_cnl[1]));

	switch (change_req.to_mod) {
	case INV_ONLY: {
	}
	case KEY_SET: {
	}
	case OP_PERM: {

		size_t      pos = after_cnl.find_first_not_of("+-ikot ");
		std::string to_grant = after_cnl.std::string::substr(
			pos, after_cnl.find_first_of(" ", pos));
		if (!to_grant.empty()) {
			channel->chmod_op(change_req.op_todo, to_grant);
		}
		// @follow-up
	}
	case TOPIC_PROTECT: {
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
		if (cmd == "PRIVMSG") {
			privmsg(after, client);
		} else if (after[0] == '#' || after[0] == '&') {
			cmd == "JOIN"    ? join(after, client)
			: cmd == "TOPIC" ? topic(after, client)
			: cmd == "MODE"  ? mode(after, client)
							 : void();
		}
	}
}