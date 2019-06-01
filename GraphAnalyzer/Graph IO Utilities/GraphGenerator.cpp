#pragma once
#include "GraphReader.h"

#include <stdlib.h> 
#include <time.h> 

config_t* genGraph(int size, float probabilityOfConnection) {
	// Preparation
	config_t* config = (config_t*)malloc(sizeof(config_t));

	config->matrixSize = size;
	config->matrix = (int*)malloc(size*size*sizeof(int));
	config->fileName = new std::string("generated randomly");

	srand(time(NULL));

	int threashhold = (int)(probabilityOfConnection * 100);

	// Filling with random connections between verticles
	for (int i = 0; i < config->matrixSize; i++)
	{
		for (int j = 0; j < config->matrixSize; j++)
		{
			int chance = rand() % 100;
			// assigns '0' or '1' which means accordingly 'disconnected' or 'connected'
			if (chance <= threashhold) {
				config->matrix[i * config->matrixSize + j] = 1;
			}
			else {
				config->matrix[i * config->matrixSize + j] = 0;
			}
		}
	}

	return config;
}