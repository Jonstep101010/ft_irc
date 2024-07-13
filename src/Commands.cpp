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

/* 
	According to chad gpt

	Most IRC servers treat channel names as case insensitive. This means:

	Typing /join #TEST and /join #test usually takes you to the same channel.
	Some servers might behave differently, but that's rare.
	In general, don't worry about capitalization in channel names on IRC.


	According to claudio

	Typically, IRC servers treat channel names as case-insensitive.
	This means that /join #TEST and /join #test would usually refer to the
	same channel. This behavior is part of the IRC protocol standard.


	for safety we will lowercase all channel names?

 */
void Server::join(std::string   channel_name,
				  Client const& client) {
	ChannelIt to_join = find_cnl(channel_name, _channels);
	if (to_join == _channels.end()) {
		Channel new_cnl(channel_name);
		new_cnl.addUser(client);
		new_cnl._clients_op[0].second = true;
		_channels.push_back(new_cnl);
	} else {
		if (to_join->_is_invite_only) { client.Output(ERR_INVITEONLYCHAN); }
		try {
			to_join->addUser(client);
		} catch (Channel::LimitReached) {
			client.Output(ERR_CHANNELISFULL);
		}
	}
}

void Server::privmsg(std::string after, Client const& client) {
	std::string dest = after.substr(0, after.find_first_of(" "));
	std::string message
		= after.substr(after.find_first_of(" ") + 2);
	if (dest[0] == '#' || dest[0] == '&') {
		ChannelIt dest_channel
			= Server::find_cnl(dest, _channels);
		if (dest_channel != _channels.end()) {
			dest_channel->Message(client, PRIVMSG_CHANNEL);
		}
	} else {
		ClientIt dest_client = Server::findnick(dest, _clients);
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
	for (ChannelIt it = _channels.begin(); it != _channels.end();
		 ++it) {
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

void Server::part(std::string after, Client const& client) {

	// Extract channel name
	std::string channel_name
		= after.substr(0, after.find_first_of(" "));

	// Find the channel
	ChannelIt at_channel = find_cnl(channel_name, _channels);

	if (at_channel == _channels.end()) {
		client.Output(ERR_NOSUCHCHANNEL);
	} else {
		// Check if client is part of the channel
		if (at_channel->findnick(client._nickname)
			!= at_channel->_clients_op.end()) {
			client.Output(PART_REPLY(client, after));
			at_channel->Message(client,
								PART_REPLY(client, after));
			at_channel->removeUser(client);
		} else {
			client.Output(ERR_NOTONCHANNEL);
		}
	}
}

// Parameters: <channel> *( "," <channel> ) <user> *( "," <user> ) [<comment>]
// [channel_name, user_name, (:comment)]
// If a "comment" is given, this will be sent instead of the default message, the nickname
// of the user issuing the KICK.
// 	KICK &Melbourne Matthew
// 	KICK #Finnish John :Speaking English
void Server::kick(std::string after, Client const& client) {
	std::vector<std::string> args = split_spaces(after);
	if (args.size() < 2) {
		client.Output(ERR_NEEDMOREPARAMS);
		return;
	}
	const std::string channel_name = args[0];
	const std::string user_name    = args[1];
	const std::string comment
		= getComment(args, client._nickname);
	ChannelIt channel = find_cnl(channel_name, _channels);
	if (channel == _channels.end()) {
		client.Output(ERR_NOSUCHCHANNEL);
		return;
	}
	if (!channel->is_operator(client)) {
		if (channel->findnick(client._nickname)
			== channel->_clients_op.end()) {
			client.Output(ERR_NOTONCHANNEL);
		} else {
			client.Output(ERR_CHANOPRIVSNEEDED);
		}
		return;
	}
	ClientIt kicked_user = Server::findnick(user_name, _clients);
	if (kicked_user == _clients.end()) {
		client.Output(ERR_USERNOTINCHANNEL);
		return;
	}
	channel->Message(client, KICK_NOTICE);
	Client& tmp = *kicked_user;
	tmp.Output(PART_REPLY(tmp, channel_name + " " + comment));
	channel->Message(
		tmp, PART_REPLY(tmp, channel_name + " " + comment));
	channel->removeUser(tmp);
}

void Server::topic(std::string after, Client const& client) {
	const std::string channel_name = get_cnl(after);
	ChannelIt channel = find_cnl(channel_name, _channels);
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

void Server::invite(std::string after, Client const& client) {
	size_t space_pos = after.find_first_of(" ");
	if (space_pos == std::string::npos) {
		return client.Output(ERR_NEEDMOREPARAMS);
	}

	const std::string invitee_nick = after.substr(0, space_pos);
	const std::string channel_name = after.substr(space_pos + 1);

	std::vector<Channel>::iterator channel
		= find_cnl(channel_name, _channels);
	if (channel == _channels.end()) {
		return client.Output(ERR_NOSUCHCHANNEL);
	}
	std::vector<Client>::iterator invitee
		= findnick(invitee_nick, _clients);
	if (invitee == _clients.end()) {
		return client.Output(ERR_NOSUCHNICK(invitee_nick));
	}

	if (channel->findnick(invitee->_nickname)
		!= channel->_clients_op.end()) {
		client.Output(ERR_USERONCHANNEL(invitee_nick));
		return;
	}
	if (!channel->is_operator(client)) {
		if (channel->findnick(client._nickname)
			== channel->_clients_op.end()) {
			client.Output(ERR_CHANOPRIVSNEEDED);
			return;
		}
		client.Output(ERR_NOTONCHANNEL);
		return;
	}
	channel->addUser(*invitee);
	// Send invite message to the invitee
	invitee->Output(INVITE);
	// Send confirmation to the inviter
	client.Output(RPL_INVITING);
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
// "MODE #channel_name -l" -> ["#channel_name", "-l"] // in case of removing limit, no optarg
// "MODE #channel_name +i" -> ["#channel_name", "+i"]
// "MODE #channel_name +t" -> ["#channel_name", "+t"]
void Server::mode(std::string after, Client const& client) {
	std::vector<std::string> args = split_spaces(after);
	if (args.size() > 3) { return; }
	// @note handle error
	ChannelIt channel = find_cnl(args[0], _channels);
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
		channel->_is_invite_only = (op_todo == ADD);
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
		channel->_topic_protection = (op_todo == ADD);
	}
	case LIMIT: {
		if (op_todo == ADD) {
			if (args.size() == 3) {
				channel->_limit = args[2][0] != '-'
									? std::atoi(args[2].c_str())
									: -1;
			}
		} else {
			if (args.size() == 2) { channel->_limit = -1; }
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
		NormalizeChannelName(after);
		if (cmd == "INVITE") {
			invite(after, client);
		} else if (cmd == "PRIVMSG") {
			privmsg(after, client);
		} else if (after[0] == '#' || after[0] == '&') {
			cmd == "JOIN"    ? join(after, client)
			: cmd == "TOPIC" ? topic(after, client)
			: cmd == "MODE"  ? mode(after, client)
			: cmd == "PART"  ? part(after, client)
			: cmd == "KICK"  ? kick(after, client)
							 : void();
		}
	}
}