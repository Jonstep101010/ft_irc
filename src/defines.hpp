#pragma once

/**
 * @brief acknowledgement of a user joining a channel
 */
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

// :NickServ!User@ClientHost PRIVMSG #channel_user :message
// we have test channel, originNick, originUser, originHost
// :originNick!originUser@originHost PRIVMSG #test :message // #originUser sends message to #test

// :originNick!originUser@originHost PRIVMSG originNick :message // #originUser sends message to originNick
#define PRIVMSG(originNick, originUser, originHost, channel,    \
				message)                                        \
	(":" + std::string(originNick) + "!"                        \
	 + std::string(originUser) + "@" + std::string(originHost)  \
	 + " PRIVMSG " + std::string(channel) + " :"                \
	 + std::string(message) + "\r\n")
