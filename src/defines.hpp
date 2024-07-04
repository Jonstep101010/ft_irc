#pragma once

#define USER_DEFAULT_NAME "tiny_user"

/**
 * @brief acknowledgement of a user joining a channel
 */
#define JOINEDREPLY                                             \
	":" + client._nickname + "@"                                    \
		+ "127.0.0.1" /* client ip @note change */ + " JOIN "   \
		+ _name /* channel name */                              \
		+ "\r\n"

#define WELCOME_MESSAGE                                         \
	":" + _server_ip + " 001 " + client._name                   \
		+ " :Welcome "                                          \
		  "to the IRC server!\r\n"

#define PING(client_name) "PING " + client_name