#pragma once
#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "GraphReader.h"

config_t* readGraphFile(std::string fileName) {
	config_t* config = (config_t*)malloc(sizeof(config_t));
	config->fileName = new std::string(fileName);

	std::ifstream file(*config->fileName);

	// Determining the dimensions of the matrix | counting lines in the file
	int totalLinesCount = 0;
	for (std::string line; std::getline(file, line);)
	{
		totalLinesCount++;
	}
	config->matrixSize = totalLinesCount;

	// Matrix generation
	int* matrix = (int*)malloc(config->matrixSize * config->matrixSize * sizeof(int**));
	for (int i = 0; i < config->matrixSize; i++)
	{
		for (int j = 0; j < config->matrixSize; j++) {
			matrix[i * config->matrixSize + j] = disconnected;
		}
	}

	// Re-reading the configuration from the file | line by line
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
				matrix[lineIndex * config->matrixSize + number] = connected;
			}
			catch (std::invalid_argument e) {
				std::cerr << "Invalid index of vertex in line: " << lineIndex << std::endl;
			}
		}
	}

	file.close();

	config->matrix = matrix;
	return config;
}

void printMatrix(config_t *config) {
	std::cout << "File \"" << *config->fileName << "\" contains the matrix of " << config->matrixSize << " verticles." << std::endl;

	for (int i = 0; i < config->matrixSize; i++) {
		for (int j = 0; j < config->matrixSize; j++)
		{
			std::cout << config->matrix[i * config->matrixSize + j] << " ";
		}
		std::cout << std::endl;
	}
}