#pragma once
#include <string>

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
	size_t pos = data.find_first_of(" :");
	if (pos != std::string::npos) {
		return data.substr(pos);
	}
	return "";
}
