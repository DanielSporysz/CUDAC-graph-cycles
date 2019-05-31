#pragma once
#include <string>
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include "../Graph IO Utilities/GraphReader.h"
#include "controller.h"
#include "cudaAnalyzer.h"

void printCycles(std::list<std::vector<int>> cycles) {
	for (std::vector<int> cycle : cycles) {
		for (int i = 0; i < cycle.size() - 1; i++)
		{
			std::cout << cycle.at(i) << "->";
		}
		std::cout << cycle.at(cycle.size() - 1) << std::endl;
	}
}

int main(int argc, char *argv[]) {
	// Confugration
	std::string defaultFileName = "./Resources/GraphFileTemplate.txt";;

	// Graph read
	std::cout << "Reading from a file: " << defaultFileName << std::endl; // DEBUG info
	config_t* config = readGraphFile(defaultFileName);

	// Analysis | Cycles detection
	std::list<std::vector<int>> cycles;
	cycles = findCycles(config->matrix, *config);

	// Results output
	std::cout << "The graph contains " << cycles.size() << " cycles." << std::endl;
	printCycles(cycles); // DEBUG  info

	// Clean up and exit
	delete config->fileName;
	free(config->matrix);
	free(config);
	return 0;
}