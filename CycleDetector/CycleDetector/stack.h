#pragma once
#include "device_launch_parameters.h"
#include "cuda_runtime.h"
#include <iostream>

__device__ int DEFAULT_STACK_SIZE = 2;

class Stack {
public:
	int *path;
	int count;
	int size;

	__device__ Stack() {
		path = (int*)malloc(sizeof(int) * DEFAULT_STACK_SIZE);
		size = DEFAULT_STACK_SIZE;
	}

	__device__ ~Stack() {
		free(path);
	}

	__device__ void push(int o) {
		if (count == size) {
			int *largerPath = (int*)malloc(2 * size * sizeof *path);
			memcpy(largerPath, path, size * sizeof *path);
			free(path);
			path = largerPath;

			size *= 2;
		}
		printf("Pushing %i", o);
		path[count++] = o;
		printf(" the count is %i\n", count);
	}

	__device__ int pop(void) {
		if (count > 0) {
			return path[--count];
		}
		else {
			return -1;
		}
	}

	__device__ int* makeCopy() {
		int* copy = (int*)malloc(count * sizeof *path);
		memcpy(copy, path, count * sizeof *path);

		return copy;
	}
};