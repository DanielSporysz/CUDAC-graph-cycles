#pragma once
#include <string>
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include <chrono>

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

void performanceTest(int startingSize, int maximumSize, int step) {
	config_t* config = genGraph(7, 1);
	printMatrix(config);
	std::list<std::vector<int>> cycles;


	// Evaluation
	auto start = std::chrono::high_resolution_clock::now();
	cycles = findCycles(config->matrix, *config);
	auto end = std::chrono::high_resolution_clock::now();

	// Print
	float time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::cout << "Time elapsed: " << time << std::endl;

	// clean up
	delete config->fileName;
	free(config->matrix);
	free(config);
}

void performanceTest2(int startingSize, int maximumSize, int step) {
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
}

int main(int argc, char *argv[]) {
	performanceTest(0,0,0);
	performanceTest2(0,0,0);
	return 0;
}