#pragma once
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

enum e_commands {
	JOIN,
	PRIVMSG,
	TOPIC,
	PART,
	KICK,
	MODE,
	INVITE_,
	NICK,
	BAD_COMMAND
};

e_commands string_to_enum(const std::string& cmd) {
	if (cmd == "JOIN") { return JOIN; }
	if (cmd == "PRIVMSG") { return PRIVMSG; }
	if (cmd == "TOPIC") { return TOPIC; }
	if (cmd == "PART") { return PART; }
	if (cmd == "KICK") { return KICK; }
	if (cmd == "MODE") { return MODE; }
	if (cmd == "INVITE") { return INVITE_; }
	if (cmd == "NICK") { return NICK; }
	return BAD_COMMAND;
}

void NormalizeChannelName(std::string& after) {
	std::string::size_type channelStartPos
		= after.find_first_of("#&");

	if (channelStartPos != std::string::npos) {
		std::string::size_type channelEndPos
			= after.find_first_of(" ", channelStartPos);

		if (channelEndPos == std::string::npos) {
			channelEndPos = after.length();
		}

		std::transform(after.begin() + channelStartPos,
					   after.begin() + channelEndPos,
					   after.begin() + channelStartPos,
					   ::tolower);
	}
}

std::string getComment(const std::vector<std::string>& args,
					   const std::string&              nick) {
	if (args.size() <= 2) { return ":" + nick; }

	std::ostringstream oss;
	if (args[2][0] != ':') { oss << ':'; }

	for (size_t i = 2; i < args.size(); ++i) {
		if (i > 2) { oss << " "; }
		oss << args[i];
	}
	return oss.str();
}

std::string get_cmd(std::string data) {
	size_t pos = data.find_first_of(" ");
	if (pos != std::string::npos) {
		return std::string(data.std::string::substr(0, pos));
	}
	return data;
}

std::string get_after_cmd(std::string data) {
	size_t pos = data.find_first_of(" ");
	if (pos != std::string::npos) {
		return data.std::string::substr(
			pos + 1, data.find_first_of("\r\n"));
	}
	return "";
}

std::string get_cnl(std::string data) {
	size_t pos = data.find_first_of("#&");
	if (pos != std::string::npos) {
		size_t end = data.find_first_of(" :", pos);
		if (end - pos > 1) {
			return data.substr(pos, end - pos);
		}
	}
	return "";
}

std::string get_additional(std::string data) {
	size_t pos = data.find_first_of(" ");
	if (pos != 0 && pos != std::string::npos) {
		pos = data.find_first_not_of(" :", pos);
		if (pos != std::string::npos && data[pos - 1] == ':') {
			return data.substr(pos);
		}
	}
	return "";
}

std::vector<std::string> split_spaces(const std::string& data) {
	std::vector<std::string> args;

	for (size_t pos = 0; pos < data.size();) {
		size_t next = data.find_first_of(" ", pos);
		if (next == std::string::npos) {
			args.push_back(data.substr(pos));
			break;
		}
		args.push_back(data.substr(pos, next - pos));
		pos = next + 1;
	}
	return args;
}