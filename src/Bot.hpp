#pragma once

#include "Server.hpp"
#include <map>
#include <string>
class Server;

class Bot {

public:
	Bot();
	~Bot();
	void initializeBot();
	void helpBot(Server& server, const std::string& target);
	void joke(Server& server, const std::string& target);
	void executeCommand(const std::string& command,
						Server&            server,
						const std::string& target);

private:
	typedef void (Bot::*BotFunctions)(Server&            server,
									  const std::string& target);
	std::string                         _botNick;
	std::map<std::string, BotFunctions> _botCommands;
};