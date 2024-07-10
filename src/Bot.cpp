#include "Bot.hpp"
#include "debug.hpp"

Bot::Bot()
	: _botNick("notSet") {
	debug(INFO, "Bot Started");
}

Bot::~Bot() {}

void Bot::helpBot() { debug(INFO, "help command called"); }

void Bot::initializeBot() {
	_botNick              = "Cool_Boot";
	_botCommands["!help"] = "salut";
}