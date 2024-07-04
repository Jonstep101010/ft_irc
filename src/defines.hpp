#pragma once

/**
 * @brief acknowledgement of a user joining a channel
 */
#define JOINEDREPLY                                             \
	":" + client._nickname + "@"                                \
		+ "127.0.0.1" /* client ip @note change */ + " JOIN "   \
		+ _name /* channel name */                              \
		+ "\r\n"

#define WELCOME_MESSAGE                                         \
	":" + _server_ip + " 001 " + client._nickname               \
		+ " :Welcome "                                          \
		  "to the IRC server!\r\n"

#define PING(client_name)                                       \
	("PING " + std::string(client_name) + "\r\n")
