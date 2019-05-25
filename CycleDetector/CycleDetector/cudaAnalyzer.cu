#pragma once
#include"cudaAnalyzer.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "vector"
#include "list"
#include <iostream>
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

__global__ void searchFrom(int** d_cycles, int* d_sizes, int testSize) {
	int tid = blockIdx.x*blockDim.x + threadIdx.x;
	
	if (tid < testSize)
	{
		d_sizes[tid] = 3;

		d_cycles[tid] = (int*)malloc(3 * sizeof(int));
		d_cycles[tid][0] = tid + 10;
		d_cycles[tid][1] = tid + 100;
		d_cycles[tid][2] = tid + 1000;
	}
}

__global__ void transferData(int* d_to, int** d_from, int* d_sizes, int verticles) {
	int id = 0;
	for (int i = 0; i < verticles; i++)
	{
		for (int j = 0; j < d_sizes[i]; j++)
		{
			d_to[id++] = d_from[i][j];
		}
	}
}

std::list<std::vector<int>> findCycles(int* matrix, config_t config) {
	std::list<std::vector<int>> cycles;

	int testSize = 5;

	// Configuration
	dim3 block(64,64);
	dim3 grid;
	grid.x = (testSize + block.x - 1) / block.x;
	grid.y = 1;

	// Data preparation
	int** d_outputs;
	int* d_sizes;
	gpuErrchk(cudaMalloc(&d_outputs, testSize * sizeof(int*)));
	gpuErrchk(cudaMalloc(&d_sizes, testSize * sizeof(int)));
	gpuErrchk(cudaMemset(d_sizes, 0, testSize * sizeof(int)));

	// Calculations
	searchFrom << <(testSize + 255) / 256, 256 >> > (d_outputs, d_sizes, testSize);
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaPeekAtLastError());

	// Data size evaluation
	int* sizes = (int*)malloc(testSize * sizeof(int));
	gpuErrchk(cudaMemcpy(sizes, d_sizes, testSize * sizeof(int), cudaMemcpyDeviceToHost));
	int count = 0;
	for (int i = 0; i < testSize; i++)
	{
		count += sizes[i];
	}

	// Data transfer
	int* d_mergedOutputs;
	gpuErrchk(cudaMalloc(&d_mergedOutputs, count * sizeof(int*)));
	transferData << <1, 1 >> > (d_mergedOutputs, d_outputs, d_sizes, testSize);
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaPeekAtLastError());

	int* outputs = (int*)malloc(count * sizeof(int));
	gpuErrchk(cudaMemcpy(outputs, d_mergedOutputs, count * sizeof(int*), cudaMemcpyDeviceToHost));

	// DEBUG print
	std::cout << "Dynamically allocated memory transfer test: " << std::endl;
	for (int i = 0; i < count; i++)
	{
		std::cout << outputs[i] << " ";
	}
	std::cout << std::endl;

	return cycles; //TODO
}