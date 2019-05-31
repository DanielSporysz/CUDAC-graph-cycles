#pragma once
#include "controller.h"
#include <list>
#include <vector>

#include "../Graph IO Utilities/GraphReader.h"

std::list<std::vector<int>> findCycles(int* matrix, config_t config);