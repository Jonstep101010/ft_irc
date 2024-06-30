#pragma once
#include <iostream>
#include <string>

class Channel {

public:
	Channel();
	Channel(Channel const& src);
	~Channel();

	Channel& operator=(Channel const& rhs);

private:
};
