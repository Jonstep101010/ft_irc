#include "Client.hpp"
#include "Server.hpp"
#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <sys/poll.h>
#include <unistd.h>
#include <vector>

Server* Server::instance = NULL;

void Server::staticWrapperSignal(int sig) {
	if (instance != 0) { instance->signalHandler(sig); }
}

void Server::signalHandler(int sig) {

	if (sig == SIGINT) {
		for (size_t i = 0; i < _pollfds.size(); ++i) {
			if (_pollfds[i].fd >= 0) { close(_pollfds[i].fd); }
		}
		_pollfds.clear();
		_running = false;
	}
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: ./IRC <Port> <Password>\n";
		return 1;
	}

	int port = std::atoi(argv[1]);

	if (port <= 0 || port > 65535) {
		std::cerr
			<< "Error: Port number out of valid range (1-65535)."
			<< std::endl;
		return 1;
	}
	if (std::string(argv[2]).empty()) {
		std::cerr << "Error: Password empty" << std::endl;
		return 1;
	}

	Server ircServer(port, argv[2]);
	signal(SIGINT, ircServer.staticWrapperSignal);
	ircServer.start();
}