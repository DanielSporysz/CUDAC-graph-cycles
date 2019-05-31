#pragma once
#include "Controller.h"
#include <list>
#include <vector>

enum mask
{
	visited = 1,
	notVisited = 0
};

std::list<std::vector<int>> findCycles(int** matrix, config_t config);
void searchFrom(int start, std::list<std::vector<int>> *cycles, int** matrix, config_t config);
void addToCycles(std::vector<int> path, std::list<std::vector<int>> *cycles);
void visitVertex(int toVisit, int start, int* visitedVerticles, std::vector<int> path, std::list<std::vector<int>> *cycles, int** matrix, config_t config);
void printCycles(std::list<std::vector<int>> cycles);