#pragma once

#define USER_DEFAULT_NAME "tiny_user"

/**
 * @brief acknowledgement of a user joining a channel
 */
#define JOINEDREPLY                                             \
	":" + client._name + "@"                                    \
		+ "127.0.0.1" /* client ip @note change */ + " JOIN "                \
		+ _name /* channel name */                              \
		+ "\r\n"