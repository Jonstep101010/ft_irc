#pragma once

#include <map>
#include <string>

class Bot {

public:
	Bot();
	~Bot();
	void initializeBot();
	void helpBot();
	void joke();
	void executeCommand(const std::string& command);

private:
	typedef void (Bot::*BotFunctions)();
	std::string                         _botNick;
	std::map<std::string, BotFunctions> _botCommands;
};