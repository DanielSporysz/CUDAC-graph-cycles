#pragma once
#include "device_launch_parameters.h"
#include "cuda_runtime.h"
#include <iostream>

class PathsContainer {
private:
	int defaultStackSize = 64;
public:
	int *paths;
	int count;
	int stackSize;

	__device__ PathsContainer() {
		count = 0;
		paths = (int*)malloc(defaultStackSize * sizeof(int));
		stackSize = defaultStackSize;
	}

	__device__ ~PathsContainer() {
		if (paths != NULL) {
			free(paths);
		}
	}

	__device__ void addPath(int* pathToAdd, int sizeToAdd) {
		if (sizeToAdd + count >= stackSize) {
			int *largerPaths = (int*)malloc(2 * stackSize * sizeof *paths);
			memcpy(largerPaths, paths, stackSize * sizeof *paths);
			free(paths);
			paths = largerPaths;

			stackSize *= 2;
		}

		memcpy(&paths[count], pathToAdd, sizeToAdd * sizeof *paths);
		count += sizeToAdd;
	}
};