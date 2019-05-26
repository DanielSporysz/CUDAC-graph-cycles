#pragma once
#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "Controller.h"

int* readGraphFile(config_t &config) {
	std::ifstream file(config.fileName);

	// Determining the dimensions of the matrix | counting lines in the file
	int totalLinesCount = 0;
	for (std::string line; std::getline(file, line);)
	{
		totalLinesCount++;
	}
	config.verticesCount = totalLinesCount;

	// Matrix (as an array) generation
	int* matrix = (int*)malloc(config.verticesCount * config.verticesCount * sizeof(int*));
	for (int i = 0; i < config.verticesCount; i++)
	{
		for (int j = 0; j < config.verticesCount; j++) {
			matrix[i*config.verticesCount + j] = disconnected;
		}
	}

	// Reading the configuration from the file | line by line
	file.clear();
	file.seekg(0, file.beg);

	int lineIndex = 0;
	for (std::string line; std::getline(file, line); lineIndex++)
	{
		// splitting the line
		std::regex rgx("\\w+");
		for (std::sregex_iterator it(line.begin(), line.end(), rgx), it_end; it != it_end; ++it) {
			try {
				int number = std::stoi((*it)[0]);
				matrix[lineIndex * config.verticesCount + number] = connected;
			}
			catch (std::invalid_argument e) {
				std::cerr << "Invalid index of vertex in line: " << lineIndex << std::endl;
			}
		}
	}

	file.close();
	return matrix;
}

void freeMatrix(int* matrix, config_t config) {
	free(matrix);
}

void printMatrix(int* matrix, config_t config) {
	std::cout << "Printing the matrix of " << config.verticesCount << " verticles." << std::endl;

	for (int i = 0; i < config.verticesCount; i++) {
		for (int j = 0; j < config.verticesCount; j++)
		{
			std::cout << matrix[i * config.verticesCount + j] << " ";
		}
		std::cout << std::endl;
	}
}