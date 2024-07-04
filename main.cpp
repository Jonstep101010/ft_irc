#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <csignal>
#include <cstdlib>
#include <sys/poll.h>
#include <unistd.h>
#include <vector>

Server* Server::instance = NULL;

void Server::staticWrapperSignal(int sig) {
	if (instance != 0) {
		instance->signalHandler(sig);
	}
}

void Server::signalHandler(int sig) {
	(void)sig;
	std::cout << "Signal Handler Freeing Server" << std::endl;
	if (_server_socket >= 0) {
		close(_server_socket);
		std::cout << "Server socket closed.\n";
	}
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd >= 0) {
			close(_pollfds[i].fd);
		}
	}
	_running = false;
	_pollfds.clear();
}

int main() {
	Channel none3;
	Server  none4;

	signal(SIGINT, none4.staticWrapperSignal);
	none4.start();
}