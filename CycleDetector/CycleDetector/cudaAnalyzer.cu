#pragma once
#include"cudaAnalyzer.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "vector"
#include "list"
#include <iostream>

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
	/*int threadsPerBlock = blockDim.x * blockDim.y * blockDim.z;
	int threadPosInBlock = threadIdx.x +
		blockDim.x * threadIdx.y +
		blockDim.x * blockDim.y * threadIdx.z;
	int blockPosInGrid = blockIdx.x +
		gridDim.x * blockDim.y +
		gridDim.x * gridDim.y * blockIdx.z;
	int tid = blockPosInGrid * threadsPerBlock + threadPosInBlock;

	int tid = blockIdx.x *blockDim.x + threadIdx.x;
	int z = 2;*/

	/*if (tid < testSize) {
		cudaMalloc(&d_cycles[tid], (tid + 1 ) * sizeof(int));
		d_sizes[tid] = tid + 1;

		for (int i = 0; i < tid; i++)
		{
			d_cycles[tid][i] = i;
		}
	}*/

	d_sizes[1] = 3;
	d_cycles[1] = (int*)malloc(3 * sizeof(int));
	d_cycles[1][0] = 2;
	d_cycles[1][1] = 1;
	d_cycles[1][2] = 0;
}

__global__ void transferData(int** d_from, int* d_to, int* d_sizes, int verticles) {
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

	// Configuration
	dim3 grid(1,1);
	dim3 block(1, 1, 1);

	int testSize = 5;

	// Data preparation
	int** d_outputs;
	int* d_sizes;
	gpuErrchk(cudaMalloc(&d_outputs, testSize * sizeof(int*)));
	gpuErrchk(cudaMalloc(&d_sizes, testSize * sizeof(int)));
	gpuErrchk(cudaMemset(d_sizes, 0, testSize * sizeof(int)));

	// Calculations
	searchFrom<<<grid, block>>>(d_outputs, d_sizes, testSize);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());

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
	transferData << <1, 1 >> > (d_outputs, d_mergedOutputs, d_sizes, testSize);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());
	int* outputs = (int*)malloc(testSize * sizeof(int));
	gpuErrchk(cudaMemcpy(outputs, d_mergedOutputs, count * sizeof(int*), cudaMemcpyDeviceToHost));

	// DEBUG print
	std::cout << "Dynamically allocated memory transfer test: " << std::endl;
	for (int i = 0; i < count; i++)
	{
		std::cout << outputs[i] << " ";
	}

	return cycles; //TODO
}