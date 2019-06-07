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
	std::list<std::vector<int>> cycles;

	// Evaluation
	auto start = std::chrono::high_resolution_clock::now();
	cycles = cpua::findCycles(config->matrix, *config);
	auto end = std::chrono::high_resolution_clock::now();

	// Print
	float time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::cout << "Time elapsed: " << time << std::endl;

	// clean up
	delete config->fileName;
	free(config->matrix);
	free(config);
}

int main(int argc, char *argv[]) {
	performanceTest(0, 0, 0);
	system("pause");
	return 0;
}