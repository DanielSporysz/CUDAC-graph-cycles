#pragma once
#include <string>
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include "GraphReader.h"
#include "Controller.h"
#include "Analyzer.h"

config_t readInputArguments(int argc, char *argv[]) {
	config_t config;

	if (argc >= 2) {
		config.fileName = argv[1];
	}
	else {
		config.fileName = "./Resources/GraphFileTemplate.txt";
	}

	return config;
}

int main(int argc, char *argv[]) {
	// Reading the configuration && Data preparation
	config_t config = readInputArguments(argc, argv);
	std::cout << "Reading from a file: " << config.fileName << std::endl; // DEBUG info

	int *matrix = readGraphFile(config);
	printMatrix(matrix, config); // DEBUG info

	// Analysis | Cycles detection
	std::list<std::vector<int>> cycles;
	cycles = findCycles(matrix, config);

	// Results output
	std::cout << "The graph contains " << cycles.size() << " cycles." << std::endl;
	printCycles(cycles); //DEBUG info

	// Clean up and exit
	free(matrix);
	system("pause");
	return 0;
}