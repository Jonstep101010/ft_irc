#pragma once

#include <iostream>
#include <string>

// Color definitions
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define ORANGE "\033[38;2;255;165;0m"
#define PINK "\033[38;2;255;192;203m"
#define PURPLE "\033[38;2;128;0;128m"
#define TEAL "\033[38;2;0;128;128m"
#define LIME "\033[38;2;50;205;50m"
#define ROYAL_BLUE "\033[38;2;65;105;225m"
#define GOLD "\033[38;2;255;215;0m"

#define SHOW_DEBUG 1
#define SHOW_SOCKET_INFO 1

// Debug message types
enum DebugType {
	ERROR,
	INFO,
	WARNING,
	TIMEOUT,
	SUCCESS,
	PING,
	PONG,
	CHANNEL,
	CLIENT,
	DEBUG,
	SOCKET,
	MESSAGE_OUTPUT
};

// Debug function declaration
inline void debug(DebugType type, const std::string& message) {
	if ((type == DEBUG && !SHOW_DEBUG)
		|| ((type == SOCKET || type == MESSAGE_OUTPUT)
			&& !SHOW_SOCKET_INFO)) {
		return;
	}
	std::string prefix;
	const char* color;

	switch (type) {
	case ERROR:
		prefix = "[ERROR]";
		color  = BRIGHT_RED;
		break;
	case INFO:
		prefix = "[INFO]";
		color  = CYAN;
		break;
	case WARNING:
		prefix = "[WARNING]";
		color  = ORANGE;
		break;
	case TIMEOUT:
		prefix = "[TIMEOUT]";
		color  = YELLOW;
		break;
	case SUCCESS:
		prefix = "[SUCCESS]";
		color  = BRIGHT_GREEN;
		break;
	case PING:
		prefix = "[PING]";
		color  = PINK;
		break;
	case SOCKET:
		prefix = "[SOCKET]";
		color  = ROYAL_BLUE;
		break;
	case MESSAGE_OUTPUT:
		prefix = "[MESSAGE OUTPUT]";
		color  = PURPLE;
		break;
	case PONG:
		prefix = "[PONG]";
		color  = BRIGHT_MAGENTA;
		break;
	case CHANNEL:
		prefix = "[CHANNEL]";
		color  = TEAL;
		break;
	case CLIENT:
		prefix = "[CLIENT]";
		color  = LIME;
		break;
	case DEBUG:
		prefix = "[DEBUG]";
		color  = GOLD;
		break;
	default:
		prefix = "[UNKNOWN]";
		color  = RESET;
		break;
	}

	time_t     now      = time(0);
	struct tm* timeinfo = localtime(&now);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);

	std::cout << color << prefix << RESET << " [" << buffer
			  << "] " << message << std::endl;
}