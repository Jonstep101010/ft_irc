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

void Bot::helpBot(Server& server, const std::string& target) {
	debug(DEBUG, "help command called");
	std::string helpList = "!help - Send you a private message "
						   "with all commands\n"
						   "!joke - Tells a joke for you\n";
	server.sendBotMessage(target, helpList);
}

void Bot::joke(Server& server, const std::string& target) {
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
void Bot::executeCommand(const std::string& command,
						 Server&            server,
						 const std::string& target) {
	std::map<std::string, BotFunctions>::iterator it
		= _botCommands.find(command);
	if (it != _botCommands.end()) {
		(this->*(it->second))(server, target);
	} else {
		debug(WARNING, "Unknown bot command: " + command);
	}
}