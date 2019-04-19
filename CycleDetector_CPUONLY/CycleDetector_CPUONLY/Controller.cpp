#pragma once
#include <string>
#include <iostream>
#include <array>
#include "GraphReader.h"
#include "Controller.h"

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
	// Reading configuration && Data preparation
	config_t config = readInputArguments(argc, argv);
	std::cout << "Reading from a file: " << config.fileName << std::endl; // DEBUG info

	int **matrix = readGraphFile(config);
	printMatrix(matrix, config); // DEBUG info

	// Analysis | Cycles detection
	// TO DO

	// Clean up and exit
	freeMatrix(matrix, config);
	system("pause");
	return 0;
}