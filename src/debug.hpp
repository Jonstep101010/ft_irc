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
#define BRIGHT_CYAN "\033[96m"
#define RED_ORANGE "\033[38;2;255;69;0m"

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
		color  = RED;
		break;
	case INFO:
		prefix = "[INFO]";
		color  = CYAN;
		break;
	case WARNING:
		prefix = "[WARNING]";
		color  = YELLOW;
		break;
	case TIMEOUT:
		prefix = "[TIMEOUT]";
		color  = RED_ORANGE;
		break;
	case SUCCESS:
		prefix = "[SUCCESS]";
		color  = GREEN;
		break;
	case PING:
		prefix = "[PING]";
		color  = YELLOW;
		break;
	case SOCKET:
		prefix = "[SOCKET]";
		color  = BLUE;
		break;
	case MESSAGE_OUTPUT:
		prefix = "[MESSAGE OUTPUT]";
		color  = MAGENTA;
		break;
	case PONG:
		prefix = "[PONG]";
		color  = MAGENTA;
		break;
	case CHANNEL:
		prefix = "[CHANNEL]";
		color  = BLUE;
		break;
	case CLIENT:
		prefix = "[CLIENT]";
		color  = GREEN;
		break;
	case DEBUG:
		prefix = "[DEBUG]";
		color  = BRIGHT_CYAN;
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