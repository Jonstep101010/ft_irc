#include "Bot.hpp"
#include "debug.hpp"
#include <string>
#include <vector>

Bot::Bot()
	: _botNick("NiceBot") {
	initializeBot();
	debug(INFO, "Bot Started");
}

Bot::~Bot() {}

void Bot::helpBot() {
	debug(DEBUG, "help command called");
	std::string helpList = "!help - Send you a private message "
						   "with all commands\n"
						   "!joke - Tells a joke for you";
}

void Bot::joke() {
	debug(DEBUG, "joke command called");
	std::vector<std::string> jokes;
	jokes.push_back("First joke");
	jokes.push_back("second joke");
}

void Bot::initializeBot() {
	_botCommands["!help"] = &Bot::helpBot;
	_botCommands["!joke"] = &Bot::joke;
}

// @follow-up We might need to pass also the client and the server
void Bot::executeCommand(const std::string& command) {
	std::map<std::string, BotFunctions>::iterator it
		= _botCommands.find(command);
	if (it != _botCommands.end()) {
		(this->*(it->second))();
	} else {
		debug(WARNING, "Unknown bot command: " + command);
	}
}