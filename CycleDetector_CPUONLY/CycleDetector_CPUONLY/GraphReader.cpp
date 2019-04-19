#pragma once
#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::vector<int>> readGraphFile(std::string graphFileName) {
	std::vector<std::vector<int>> matrix; // Stores the graph
	std::ifstream file(graphFileName);

	// Reading line by line
	int linesCounter = 0;
	std::string line;
	for (; std::getline(file, line); linesCounter++)
	{
		std::vector<int> connections;
		matrix.push_back(connections);
		
		// line splitting 
		std::regex rgx("\\w+");
		for (std::sregex_iterator it(line.begin(), line.end(), rgx), it_end; it != it_end; ++it) {
			try {
				int number = std::stoi((*it)[0]);
				matrix[linesCounter].push_back(number);
			}
			catch (std::invalid_argument e) {
				std::cerr << "Invalid index of vertex!" << std::endl;
			}
		}
	}

	return matrix;
}

void printMatrix(std::vector<std::vector<int>> matrix) {
	std::cout << "Printing a matrix of " << matrix.size() << " verticles." << std::endl;
	
	for (std::vector<int> connections : matrix) {
		for (int nextVertex : connections) {
			std::cout << nextVertex << " ";
		}
		std::cout << std::endl;
	}
}