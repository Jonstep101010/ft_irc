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
	void roll(Server& server, const std::string& target,
			  const std::string& command,
			  const std::string& channel_name);
	void executeCommand(const std::string& command,
						Server&            server,
						const std::string& target,
						const std::string& channel_name);

private:
	typedef void (Bot::*BotFunctions)(Server&            server,
									  const std::string& target);
	// clang-format off
	typedef std::map<std::string, BotFunctions>::iterator BotFunction;
	// clang-format on
	std::string                         _botNick;
	std::map<std::string, BotFunctions> _botCommands;
};