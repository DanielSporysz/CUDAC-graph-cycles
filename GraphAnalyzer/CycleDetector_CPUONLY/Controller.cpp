#pragma once
#include <string>
#include <iostream>
#include <array>
#include <list>
#include <vector>
#include <chrono>

#include "Controller.h"
#include "Analyzer.h"
#include "../Graph IO Utilities/GraphReader.h"

void printCycles(std::list<std::vector<int>> cycles) {
	int iteration = 0;
	for (std::vector<int> cycle : cycles) {
		if (iteration >= 10) {
			std::cout << "Too many cycles. Only the head of the container of cycles has been displayed!" << std::endl;
			break;
		}

		for (int i = 0; i < cycle.size() - 1; i++)
		{
			std::cout << cycle.at(i) << "->";
		}
		std::cout << cycle.at(cycle.size() - 1) << std::endl;

		iteration++;
	}
}

void performanceTest(int startingSize, int maximumSize, int step) {
	config_t* config = genGraph(7, 1);
	printMatrix(config);
	std::list<std::vector<int>> cycles;


	// Evaluation
	auto start = std::chrono::high_resolution_clock::now();
	cycles = cpua::findCycles(config->matrix, *config);
	auto end = std::chrono::high_resolution_clock::now();

	// Print
	float time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Time elapsed: " << time << " milliseconds." << std::endl;

	// Results output
	std::cout << "The graph contains " << cycles.size() << " cycles." << std::endl;
	printCycles(cycles); // DEBUG  info

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
	printMatrix(config);

	// Analysis | Cycles detection
	std::list<std::vector<int>> cycles;
	cycles = cpua::findCycles(config->matrix, *config);

	// Results output
	std::cout << "The graph contains " << cycles.size() << " cycles." << std::endl;
	printCycles(cycles); // DEBUG  info

	// Clean up and exit
	delete config->fileName;
	free(config->matrix);
	free(config);
}

int main(int argc, char *argv[]) {
	performanceTest(0, 0, 0);
	//performanceTest2(0,0,0);
	system("pause");
	return 0;
}