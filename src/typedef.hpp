#pragma once

#include "Channel.hpp"
#include "Client.hpp"
#include <vector>

class Channel;
class Client;

// clang-format off
typedef std::vector<std::pair<Client, bool> >::iterator ClientOpIt;
typedef std::vector<std::pair<Client, bool> > ClientOp;
// clang-format on
typedef std::vector<Channel>::iterator ChannelIt;
typedef std::vector<Client>::iterator ClientIt;