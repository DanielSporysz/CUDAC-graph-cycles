#pragma once
#include "controller.h"
#include <list>
#include <vector>

#include "../Graph IO Utilities/GraphReader.h"

#ifndef _MASK__
#define _MASK__

enum mask
{
	visited = 1,
	notVisited = 0
};

#endif _MASK__

std::list<std::vector<int>> findCycles(int* matrix, config_t config);