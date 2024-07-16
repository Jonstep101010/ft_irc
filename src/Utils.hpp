#pragma once
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

enum e_commands {
	e_JOIN,
	e_PRIVMSG,
	e_TOPIC,
	e_PART,
	e_KICK,
	e_MODE,
	e_INVITE,
	e_NICK,
	e_BAD_COMMAND
};

e_commands string_to_enum(const std::string& cmd) {
	if (cmd == "JOIN") { return e_JOIN; }
	if (cmd == "PRIVMSG") { return e_PRIVMSG; }
	if (cmd == "TOPIC") { return e_TOPIC; }
	if (cmd == "PART") { return e_PART; }
	if (cmd == "KICK") { return e_KICK; }
	if (cmd == "MODE") { return e_MODE; }
	if (cmd == "INVITE") { return e_INVITE; }
	if (cmd == "NICK") { return e_NICK; }
	return e_BAD_COMMAND;
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
		if (i > 2) {
			oss << " ";
		} // Add space before each word except the first
		oss << args[i];
	}
	return oss.str();
}

std::string get_cmd(std::string data) {
	size_t pos = data.find_first_of(" ");
	if (pos != std::string::npos) {
		return std::string(data.std::string::substr(0, pos));
	}
	return "";
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

/**
 * @brief split data into substrings, add each to vector
 * 
 * @param data to split by spaces
 * @return std::vector<std::string>& 
 */
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