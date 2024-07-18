#pragma once

/**
 * @brief acknowledgement of a user joining a channel
 */

#define MAX_INPUT 510

#define JOINEDREPLY                                             \
	":" + client._nickname + "@"                                \
		+ client._ip /* client ip @note change */ + " JOIN "    \
		+ _name /* (prefix +)channel name */                    \
		+ "\r\n"

#define JOINED_NOTICE                                           \
	":" + client._nickname + "!" + client._name + "@"           \
		+ client._ip + " JOIN :"                                \
		+ _name /* (prefix +)channel name */ + "\r\n"

#define WELCOME_MESSAGE                                         \
	":" + _server_ip + " 001 " + client._nickname               \
		+ " :Welcome "                                          \
		  "to the IRC server!\r\n"

#define PING(client_name)                                       \
	("PING " + std::string(client_name) + "\r\n")

// :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?// Wiz sends message to Angel
// Angel input is:
// PRIVMSG Wiz :Are you receiving this message ?

// :originNick!originUser@originHost PRIVMSG originNick :message // #originUser sends message to originNick
#define PRIVMSG_USER                                            \
	":" + client._nickname + "!" + client._name + "@"           \
		+ client._ip + " PRIVMSG " + dest_client->_nickname     \
		+ " :" + message + "\r\n"

// :originNick!originUser@originHost PRIVMSG #test :message // #originUser sends message to #test
#define PRIVMSG_CHANNEL                                         \
	":" + client._nickname + "!" + client._name + "@"           \
		+ client._ip + " PRIVMSG " + dest_channel->_name + " :" \
		+ message + "\r\n"

// topic reply
#define RPL_NOTOPIC                                             \
	":" + _server_ip + " 331 " + client._nickname + " "         \
		+ channel_name + " :No topic is set\r\n"

// channel limit reached
#define ERR_CHANNELISFULL                                       \
	":" + _server_ip + " 471 " + client._nickname + " "         \
		+ channel_name + " :Cannot join channel (+l)\r\n"

#define RPL_TOPIC(channel)                                      \
	":" + _server_ip + " 332 " + client._nickname + " "         \
		+ channel_name + " :" + channel->_topic + "\r\n"

#define ERR_CHANOPRIVSNEEDED                                    \
	":" + _server_ip + " 482 " + client._nickname + " "         \
		+ channel_name + " :You're not channel operator\r\n"

#define ERR_PASSWDMISMATCH                                      \
	":" + _server_ip + " 464 * :Password incorrect\r\n"

#define ERR_NEEDMOREPARAMS(cmd)                                 \
	":" + _server_ip + " 461 * " + cmd                          \
		+ " :Not enough parameters\r\n"

// :john!username@hostname PART #test :Goodbye!
#define PART_REPLY(client, current_channel_name)                \
	":" + client._nickname + "!" + client._name + "@"           \
		+ client._ip + " PART " + current_channel_name + "\r\n"

// "<channel name> :No such channel"
#define ERR_NOSUCHCHANNEL(cmd)                                  \
	":" + _server_ip + " 403  * " + cmd + " :No such channel\r\n"

// "<client name> <channel name> :You're not on that channel"
#define ERR_NOTONCHANNEL                                        \
	":" + _server_ip + " 442  * PART"                           \
		+ " :You aren't on that channel\r\n"

#define ERR_USERNOTINCHANNEL(_server_ip, user_name)             \
	":" + _server_ip + " 441 " + user_name                      \
		+ " :They aren't on that channel" + "\r\n"

#define KICK_NOTICE                                             \
	":" + client._nickname + "!" + client._name + "@"           \
		+ client._ip + " KICK " + channel_name + " "            \
		+ user_name + " " + comment + "\r\n"

#define ERR_NOSUCHNICK(invitee_nick)                            \
	":" + _server_ip + " 401 " + invitee_nick                   \
		+ " :No such nick/channel\r\n"

#define ERR_USERONCHANNEL(invitee_nick)                         \
	":" + _server_ip + " 443 " + invitee_nick + " "             \
		+ channel->_name + " : is already on channel\r\n "

#define INVITE                                                  \
	":" + client._nickname + " INVITE " + invitee->_nickname    \
		+ " " + channel->_name + "\r\n"

#define RPL_INVITING                                            \
	"341 " + client._nickname + " " + invitee->_nickname + " "  \
		+ channel->_name + "\r\n"

#define ERR_INVITEONLYCHAN                                      \
	":" + _server_ip + " 473  * JOIN"                           \
		+ " :Cannot join channel (+i)\r\n"

#define ERR_ERRONEUSNICKNAME(client)                            \
	":" + _server_ip + " 432 " + client._nickname               \
		+ " :Erroneous nickname\r\n"

#define ERR_CHANNELNAMETOOLONG                                  \
	":" + _server_ip + " 403 " + client._nickname + " "         \
		+ channel_name + " :Channel name is too long" + "\r\n"

#define ERR_BADCHANNELKEY                                       \
	":" + _server_ip + " 461 * JOIN"                            \
		+ " :Cannot join channel (+k)\r\n"

#define ERR_NONICKNAMEGIVEN                                     \
	":" + _server_ip + " 431 " + client._nickname               \
		+ " :Nickname not given\r\n"

#define ERR_NICKNAMEINUSE                                       \
	":" + _server_ip + " 433 " + client._nickname               \
		+ " :Nickname already in use\r\n"

//:WiZ!jto@tolsun.oulu.fi NICK Kilroy
#define NICK_REPLY                                              \
	":" + oldNick + "!" + client._name + "@" + client._ip       \
		+ " NICK " + after + "\r\n"

#define ERR_KEYSET                                              \
	":" + _server_ip + " 467 " + client._nickname               \
		+ " :Channel key already set\r\n"

#define ERR_UNKNOWNMODE(_char)                                  \
	":" + _server_ip + " 472 " + client._nickname + " " + _char \
		+ " :is unknown mode char to me for " + channel_name    \
		+ "\r\n"

#define ERR_NORECIPIENT(_char)                                  \
	":" + _server_ip + " 411 " _char + " :No recipient given "  \
		+ _char + "\r\n"

#define ERR_NOTEXTTOSEND                                        \
	":" + _server_ip + " 412 * :No text to send\r\n"

#define ERR_CANNOTSENDTOCHAN(channel_name)                      \
	":" + _server_ip + " 404 " + client._nickname + " "         \
		+ channel_name + " :Cannot send to channel\r\n"