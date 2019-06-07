#pragma once
#include <cuda.h>
#include <cuda_runtime_api.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <device_functions.h>

#include <iostream>
#include <list>
#include <vector>

#include "../Graph IO Utilities/GraphReader.h"

#include"cudaAnalyzer.h"
#include "stack.h"
#include "PathsContainer.h"


#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}

__device__ void visitVertex(int toVisit, int destination, Stack* path, int* visitedVertices, PathsContainer* cycles, int* matrix, int count) {
	path->push(toVisit);
	visitedVertices[toVisit] = visited;

	for (int i = 0; i < count; i++)
	{
		if (matrix[toVisit * count + i] == connected) {
			if (i == destination) { // Found the destination
				path->push(destination);
				int* pathToAdd = path->makeCopy();
				cycles->addPath(pathToAdd, path->count);
				free(pathToAdd);
				path->pop();
			}
			else if (visitedVertices[i] == notVisited) { // Look futher
				visitVertex(i, destination, path, visitedVertices, cycles, matrix, count);
			}
		}
	}

	path->pop();
	visitedVertices[toVisit] = notVisited;
}

__global__ void beginVisting(PathsContainer* d_outputs, int* d_matrix, config_t d_config) {
	int tid = blockIdx.x*blockDim.x + threadIdx.x;
	int count = d_config.matrixSize;

	if (tid < d_config.matrixSize)
	{
		// PreAnalysis data preparations
		int* visitedVerticles = (int*)malloc(count * sizeof(int));
		for (int i = 0; i < count; i++) {
			visitedVerticles[i] = notVisited;
		}

		Stack path;
		new(&d_outputs[tid]) PathsContainer;

		// Analysis
		visitVertex(tid, tid, &path, visitedVerticles, &d_outputs[tid], d_matrix, count);

		free(visitedVerticles);
	}
}


__global__ void getOutputSize(PathsContainer* d_outputs, int* outputSize, config_t d_config) {
	*outputSize = 0;
	for (int i = 0; i < d_config.matrixSize; i++)
	{
		*outputSize += d_outputs[i].count;
	}
}

__global__ void transferOutputs(int* cycles, PathsContainer* d_outputs, config_t config) {
	int offset = 0;
	for (int i = 0; i < config.matrixSize; i++)
	{
		memcpy(&cycles[offset], d_outputs[i].paths, d_outputs[i].count * sizeof(*cycles));
		offset += d_outputs[i].count;
	}
}

__global__ void freePathsContainers(PathsContainer* d_outputs, int count) {
	for (int i = 0; i < count; i++)
	{
		if (&d_outputs[i] != NULL) {
			delete &d_outputs[i];
		}

	}
}

std::list<std::vector<int>> convertToList(int* mergedCycles, int count) {
	std::list<std::vector<int>> cycles;

	std::vector<int> *tmp = new std::vector<int>();
	int head = mergedCycles[0];
	tmp->push_back(head);

	for (int i = 1; i < count; i++)
	{
		tmp->push_back(mergedCycles[i]);

		if (head == mergedCycles[i]) {
			cycles.push_back(*tmp);
			tmp = new std::vector<int>();

			if (i + 1 < count) {
				head = mergedCycles[i + 1];
				tmp->push_back(head);
				i++;
			}
		}
	}

	return cycles;
}

std::list<std::vector<int>> findCycles(int* matrix, config_t config) {
	// Data preparation
	PathsContainer* d_outputs;
	int* d_matrix;
	int matrixSize = config.matrixSize * config.matrixSize * sizeof(*matrix);
	gpuErrchk(cudaMalloc(&d_matrix, matrixSize));
	gpuErrchk(cudaMemcpy(d_matrix, matrix, matrixSize, cudaMemcpyHostToDevice));
	gpuErrchk(cudaMalloc(&d_outputs, config.matrixSize * sizeof(PathsContainer)));

	// Calculations
	beginVisting << <(config.matrixSize + 255) / 256, 256>> > (d_outputs, d_matrix, config);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());

	// Data size evaluation
	int* d_outputSize;
	gpuErrchk(cudaMalloc(&d_outputSize, sizeof(int)));

	getOutputSize << <1, 1 >> > (d_outputs, d_outputSize, config);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());

	int* outputSize = (int*)malloc(sizeof(int));
	gpuErrchk(cudaMemcpy(outputSize, d_outputSize, sizeof(*outputSize), cudaMemcpyDeviceToHost));

	// Data transfer
	int *d_cycles;
	gpuErrchk(cudaMalloc(&d_cycles, *outputSize * sizeof(*matrix)));
	transferOutputs << <1, 1 >> > (d_cycles, d_outputs, config);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());

	int* mergedCycles = (int*)malloc(*outputSize * sizeof(*matrix));
	gpuErrchk(cudaMemcpy(mergedCycles, d_cycles, *outputSize * sizeof(*matrix), cudaMemcpyDeviceToHost));

	// Conversion
	std::list<std::vector<int>> cycles;
	cycles = convertToList(mergedCycles, *outputSize);

	// Clean up
	gpuErrchk(cudaFree(d_matrix));
	gpuErrchk(cudaFree(d_cycles));
	freePathsContainers << <1, 1 >> > (d_outputs, config.matrixSize);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaFree(d_outputSize));
	gpuErrchk(cudaFree(d_outputs));

	free(mergedCycles);
	free(outputSize);

	return cycles;
}