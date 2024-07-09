#pragma once

#include "Client.hpp"
#include <vector>

// clang-format off
typedef std::vector<std::pair<Client, bool> >::iterator ClientOpIt;
typedef std::vector<std::pair<Client, bool> > ClientOp;
// clang-format on