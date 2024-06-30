#pragma once
#include <iostream>
#include <string>

class User {

public:
	User();
	User(User const& src);
	~User();

	User& operator=(User const& rhs);

private:
};
