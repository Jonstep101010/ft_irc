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

	if (sig == SIGINT) {
		// fds to close
		int fds[] = {27, 23, 22, 20, 18, 0, 1, 2};
		for (size_t i = 0; i < sizeof(fds) / sizeof(int); ++i) {
			close(fds[i]);
		}

		// client sockets
		for (size_t i = 0; i < _pollfds.size(); ++i) {
			if (_pollfds[i].fd >= 0) {
				close(_pollfds[i].fd);
			}
		}

		// free pollfds
		_pollfds.clear();
		_running = false;
	}
}

int main() {
	Channel none3;
	Server  none4;

	signal(SIGINT, none4.staticWrapperSignal);
	none4.start();
}