#include "Bot.hpp"
#include "debug.hpp"
#include <cstdlib>
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
	jokes.push_back("Why don't scientists trust atoms? Because "
					"they make up everything!");
	jokes.push_back("Why did the scarecrow win an award? He was "
					"outstanding in his field!");
	jokes.push_back("Why don't eggs tell jokes? They'd crack "
					"each other up!");
	jokes.push_back("Why don't skeletons fight each other? They "
					"don't have the guts!");
	jokes.push_back("Why did the bicycle fall over? Because it "
					"was two-tired!");
	server.sendBotMessage(target,
						  jokes.at(rand() % jokes.size()));
}

void Bot::initializeBot() {
	_botCommands["!help"] = &Bot::helpBot;
	_botCommands["!joke"] = &Bot::joke;
}

// @follow-up We might need to pass also the client and the server
void Bot::executeCommand(const std::string& command,
						 Server&            server,
						 const std::string& target) {
	BotFunction it = _botCommands.find(command);
	if (it != _botCommands.end()) {
		(this->*(it->second))(server, target);
	} else {
		debug(WARNING, "Unknown bot command: " + command);
	}
}