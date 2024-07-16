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
	std::string helpList
		= "!help - Send you a private message "
		  "with all commands\n"
		  "!joke - Tells a joke for you\n"
		  "!roll [NdM] (e.g., !roll 2d6) - Rolling dice";
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

std::string get_params(std::string data) {
	size_t pos = data.find_first_of(" ");
	if (pos != std::string::npos) {
		return data.std::string::substr(
			pos + 1, data.find_first_of(" ") - 1);
	}
	return "";
}

void Bot::roll(Server& server, const std::string& target,
			   const std::string& command,
			   const std::string& channel_name) {
	debug(DEBUG, "roll command called");
	std::string params = get_params(command);
	int         dice   = 1;
	int         sides  = 6;

	std::istringstream iss(params);
	char               d = 0;
	if (iss >> dice >> d >> sides) {
		if (d != 'd' && d != 'D') {
			server.sendBotMessage(
				target, "Usage: !roll [NdM] (e.g., !roll 2d6)");
			return;
		}
	} else if (params.empty()) {
		server.sendBotMessage(
			target, "Usage: !roll [NdM] (e.g., !roll 2d6)");
		return;
	}

	if (dice < 1 || dice > 100 || sides < 2 || sides > 100) {
		server.sendBotMessage(target,
							  "Invalid dice or sides. Use "
							  "1-100 dice with 2-100 sides.");
		return;
	}

	int               total = 0;
	std::stringstream result;
	result << target << " rolled " << dice << "d" << sides
		   << ": ";

	for (int i = 0; i < dice; ++i) {
		int roll = (rand() % sides) + 1;
		total += roll;
		result << roll;
		if (i < dice - 1) result << ", ";
	}

	result << ". Total: " << total;
	server.sendBotMessage(channel_name, result.str());
	// debug(ERROR, params);
	// server.sendBotMessage(target, "I will roll the dice chief");
}

void Bot::initializeBot() {
	_botCommands["!help"] = &Bot::helpBot;
	_botCommands["!joke"] = &Bot::joke;
}

// @follow-up We might need to pass also the client and the server
void Bot::executeCommand(const std::string& command,
						 Server&            server,
						 const std::string& target,
						 const std::string& channel_name) {
	if (command.rfind("!roll", 0) == 0) {
		roll(server, target, command, channel_name);
	} else {
		BotFunction it = _botCommands.find(command);
		if (it != _botCommands.end()) {
			(this->*(it->second))(server, target);
		} else {
			if ((command.rfind("!help", 0) != 0)
				|| command.rfind("!joke", 0) != 0
				|| command.rfind("!roll", 0) != 0) {
				debug(WARNING,
					  "Unknown bot command: " + command);
				server.sendBotMessage(target, "Unknown command: "
												  + command);
				return;
			}
			server.sendBotMessage(
				target, "Commands as !help and !joke does "
						"not require arguments");
		}
	}
}