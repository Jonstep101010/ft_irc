#pragma once

#include <map>
#include <string>

class Bot {

public:
	Bot();
	~Bot();
	void initializeBot();
	void helpBot();

private:
	std::string                        _botNick;
	std::map<std::string, std::string> _botCommands;
};