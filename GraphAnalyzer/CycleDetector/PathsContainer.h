#pragma once
#include "device_launch_parameters.h"
#include "cuda_runtime.h"
#include <iostream>

class PathsContainer {
public:
	int *paths;
	int size;

	__device__ PathsContainer() {
		size = 0;
	}

	__device__ ~PathsContainer() {
		/*if (paths != NULL) {
			free(paths);
		}*/
	}

	__device__ void addPath(int* pathToAdd, int sizeToAdd) {
		int* newPaths = (int*)malloc((size + sizeToAdd) * sizeof *paths);

		memcpy(newPaths, paths, size * sizeof *paths);
		memcpy(&newPaths[size], pathToAdd, sizeToAdd * sizeof *paths);

		free(paths);
		paths = newPaths;
		size += sizeToAdd;
	}
};