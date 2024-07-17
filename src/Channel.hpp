#pragma once
#include "Client.hpp"
#include "defines.hpp"
#include "typedef.hpp"
#include <algorithm>
#include <string>
#include <vector>

#define CHANNEL_NAME_LEN 50

typedef enum e_mode_op {
	ADD = '+',
	RM  = '-',
} MODE_OP;

class Client;

class Channel {

public:
	/*
	** ------------------------------- CONSTRUCTOR --------------------------------
	*/
	Channel(std::string const& name)
		: _name(name)
		, _limit(-1)
		, _is_invite_only()
		, _topic_protection() {}

	Channel()
		: _limit(-1), _is_invite_only(), _topic_protection() {}

	Channel(const Channel& src)
		: _name(src._name)
		, _topic(src._topic)
		, _clients_op(src._clients_op)
		, _limit(src._limit)
		, _is_invite_only(src._is_invite_only)
		, _topic_protection(src._topic_protection)
		, _key(src._key) {}
	~Channel() {}

	bool operator==(const Client& other) const {
		return this->_name == other._name;
	}

	bool is_operator(Client const& client) {
		ClientOpIt it = std::find_if(_clients_op.begin(),
									 _clients_op.end(),
									 CompareClient(client));
		if (it != _clients_op.end()) { return it->second; }
		return false;
	}

	// @follow-up add method to get a mutable reference to a Client operator status

	/// USER MANAGEMENT ///
	// add user to channel
	void addUser(Client const& client);

	class LimitReached : public std::exception {};
	// remove user from channel
	void removeUser(Client const& client);

	// send message to all Clients in channel
	void Message(Client const&      origin,
				 std::string const& message);

	/// CHANNEL MANAGEMENT ///
	// set channel topic
	void setTopic(std::string& new_topic);
	// set mode method (will be directed to member functions)
	// @follow-up remove this/move ::mode here?
	void setMode(std::string const& mode);
	// @todo add mode specific functions

	void chmod_op(MODE_OP change, std::string const& nick,
				  Client const&      client,
				  std::string const& server_ip) {
		ClientOpIt it = findnick(nick);
		if (it != _clients_op.end()) {
			it->second = (change == ADD);
		} else {
			client.Output(ERR_USERNOTINCHANNEL(server_ip, nick));
		}
	}

	void setClientNick(std::string& old, std::string& newName) {
		//change old name inside _clients_op to new
		ClientOpIt it = findnick(old);
		if (it != _clients_op.end()) {
			it->first._nickname = newName;
		}
	}

	friend class Server;

private:
	const std::string _name;
	std::string       _topic;
	// clang-format off
	std::vector<std::pair<Client, bool> > _clients_op;
	// clang-format on

	class CompareClient {
		const Client& targetClient;

	public:
		CompareClient(const Client& client)
			: targetClient(client) {}

		bool operator()(
			const std::pair<Client, bool>& element) const {
			return element.first == targetClient;
		}
	};
	class CompareOperator {
		const bool target_op;

	public:
		CompareOperator(const bool is_op)
			: target_op(is_op) {}

		bool operator()(
			const std::pair<Client, bool>& element) const {
			return element.second == target_op;
		}
	};

	ClientOpIt findnick(std::string const& client_nick) {
		for (ClientOpIt it = _clients_op.begin();
			 it != _clients_op.end(); ++it) {
			if (it->first._nickname == client_nick) {
				return it;
			}
		}
		return _clients_op.end();
	}

	int         _limit;
	bool        _is_invite_only;
	bool        _topic_protection;
	std::string _key; // if empty, no key required
};
