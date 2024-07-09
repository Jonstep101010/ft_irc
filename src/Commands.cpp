#include "Channel.hpp"
#include "Client.hpp"
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
#include <vector>

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
		debug(DEBUG, "removing from channel: " + it->_name);
		it->removeUser(client);
	}
	// remove user from clients
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd == client._ClientSocket) {
			_pollfds.erase(_pollfds.begin() + i);
		}
	}
	close(client._ClientSocket);
	_clients.erase(
		std::find(_clients.begin(), _clients.end(), client));
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
	LIMIT         = 'l',
} MODES;

// remove? @audit-info
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

// format of "MODE #channel_name opstring (optarg)" -> ["#channel_name", "opstring" (, "optarg")]
// "MODE #channel_name +o nickname" -> ["#channel_name", "+l", /* needs prefix */ "username"]
// "MODE #channel_name +k password" -> ["#channel_name", "+k", /* needs prefix */ "password"]
// "MODE #channel_name +l number" -> ["#channel_name", "+l", /* needs prefix & needs to be positive */ "num"]
// "MODE #channel_name +i" -> ["#channel_name", "+i"]
// "MODE #channel_name +t" -> ["#channel_name", "+t"]
void Server::mode(std::string after, Client const& client) {
	std::vector<std::string> args = split_spaces(after);
	if (args.size() > 3) { return; }
	// @note handle error
	std::vector<Channel>::iterator channel
		= find_cnl(args[0], _channels);
	if (channel == _channels.end()) { return; }
	// @todo handle error, channel not existing, user not being member,...
	if (!channel->is_operator(client)) { return; }
	// args[0] = channel_name
	if ((args[1][0] != ADD && args[1][0] != RM)
		|| !strchr("ikotl", args[1][1]) || args[1][2]) {
		return;
	}
	if (args.size() == 3 && args[1][1] != 'k'
		&& args[1][1] != 'l' && args[1][1] != 'o') {
		// invalid number of arguments
		return;
	}
	debug(DEBUG, "MODE vec[0]'" + args[0] + "'");
	debug(DEBUG, "MODE vec[1]'" + args[1] + "'");
	if (args.size() == 3) {
		debug(DEBUG, "MODE vec[2]'" + args[2] + "'");
	}
	// @note priviledged access
	// @follow-up more sophisticated parsing/checking
	// @todo handle error
	MODE_OP op_todo
		= static_cast<MODE_OP>(static_cast<int>(args[1][0]));
	MODES to_mod
		= static_cast<MODES>(static_cast<int>(args[1][1]));

	switch (to_mod) {
	case INV_ONLY: {
	}
	case KEY_SET: {
	}
	case OP_PERM: {
		if (!args[2].empty()) {
			channel->chmod_op(op_todo, args[2]);
		}
		// @follow-up
	}
	case TOPIC_PROTECT: {
	}
	case LIMIT: {
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