#pragma once
#include <string>

struct config_t {
	std::string *fileName;
	int matrixSize;
	int *matrix;
};

enum markings
{
	connected = 1,
	disconnected = 0
};

config_t* readGraphFile(std::string fileName);
void printMatrix(config_t *config);