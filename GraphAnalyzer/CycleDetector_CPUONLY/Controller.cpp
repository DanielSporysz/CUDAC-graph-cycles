#pragma once
#include <string>
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include "Controller.h"
#include "Analyzer.h"

#include "../Graph IO Utilities/GraphReader.h"

int main(int argc, char *argv[]) {
	// Confugration
	std::string defaultFileName = "./Resources/GraphFileTemplate.txt";;

	// Graph read
	std::cout << "Reading from a file: " << defaultFileName << std::endl; // DEBUG info
	config_t* config = readGraphFile(defaultFileName);

	// Analysis | Cycles detection
	std::list<std::vector<int>> cycles;
	cycles = cpua::findCycles(config->matrix, *config);

	// Results output
	std::cout << "The graph contains " << cycles.size() << " cycles." << std::endl;
	cpua::printCycles(cycles); //DEBUG info

	// Clean up and exit
	delete config->fileName;
	free(config->matrix);
	free(config);

	system("pause");
	return 0;
}