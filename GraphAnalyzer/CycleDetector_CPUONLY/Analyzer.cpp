#pragma once
#include "Controller.h"
#include "Analyzer.h"
#include <list>
#include <vector>
#include <vector>
#include <iostream>

namespace cpua {
	std::list<std::vector<int>> findCycles(int* matrix, config_t config) {
		std::list<std::vector<int>> cycles;
		std::list<std::vector<int>> *pCycles = &cycles;

		for (int i = 0; i < config.matrixSize; i++)
		{
			searchFrom(i, pCycles, matrix, config);
		}

		return cycles;
	}

	void searchFrom(int start, std::list<std::vector<int>> *cycles, int* matrix, config_t config) {
		// Data preparation
		std::vector<int> path;

		int* visitedVerticles = (int*)malloc(config.matrixSize * sizeof(int));
		for (int i = 0; i < config.matrixSize; i++) {
			visitedVerticles[i] = notVisited;
		}

		visitVertex(start, start, visitedVerticles, path, cycles, matrix, config);

		free(visitedVerticles);
	}

	void visitVertex(int toVisit, int destination, int* visitedVerticles, std::vector<int> path, std::list<std::vector<int>> *cycles, int* matrix, config_t config) {
		path.push_back(toVisit);
		visitedVerticles[toVisit] = visited;

		for (int i = 0; i < config.matrixSize; i++)
		{
			if (matrix[toVisit * config.matrixSize + i] == connected) {
				if (i == destination) { // Found the destination
					path.push_back(destination);
					addToCycles(path, cycles);
					path.pop_back();
				}
				else if (visitedVerticles[i] == notVisited) { // Look futher
					visitVertex(i, destination, visitedVerticles, path, cycles, matrix, config);
				}
			}
		}

		path.pop_back();
		visitedVerticles[toVisit] = notVisited;
	}

	void addToCycles(std::vector<int> path, std::list<std::vector<int>> *cycles) {
		std::vector<int> pathToAdd;

		for (int i = 0; i < path.size(); i++) { // Making a copy
			pathToAdd.push_back(path[i]);
		}

		cycles->push_front(pathToAdd);
	}

	void printCycles(std::list<std::vector<int>> cycles) {
		for (std::vector<int> path : cycles)
		{
			int pathIterator = 0;
			for (int vertex : path) {
				std::cout << vertex;

				if (pathIterator++ < path.size() - 1) { // don't print the last one
					std::cout << " -> ";
				}

			}

			std::cout << std::endl;
		}
	}
}
