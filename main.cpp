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
	for (size_t i = 0; i < _pollfds.size(); ++i) {
		if (_pollfds[i].fd >= 0) {
			close(_pollfds[i].fd);
		}
		std::cout << "Client socket closed. at "
				  << _pollfds[i].fd << std::endl;
		;
	}
	// HELL ON EARTH
	close(0);
	close(1);
	close(2);
	close(18);
	close(20);
	close(21);
	close(22);
	close(23);
	close(27);
	_running = false;
	_pollfds.clear();
}

int main() {
	Channel none3;
	Server  none4;

	signal(SIGINT, none4.staticWrapperSignal);
	none4.start();
}