#pragma once
#include"cudaAnalyzer.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "vector"
#include "list"
#include <iostream>

__device__ int* d_array;

__global__ void searchFrom(int* adr, int testSize) {
	int threadsPerBlock = blockDim.x * blockDim.y * blockDim.z;
	int threadPosInBlock = threadIdx.x +
		blockDim.x * threadIdx.y +
		blockDim.x * blockDim.y * threadIdx.z;
	int blockPosInGrid = blockIdx.x +
		gridDim.x * blockDim.y +
		gridDim.x * gridDim.y * blockIdx.z;
	int tid = blockPosInGrid * threadsPerBlock + threadPosInBlock;

	if (tid < testSize) {
		adr[tid] = 1;
	}
}

std::list<std::vector<int>> findCycles(int* matrix, config_t config) {
	std::list<std::vector<int>> cycles;

	dim3 block(8, 8, 8);
	dim3 grid(16,16);
	int testSize = 5;

	cudaMalloc(&d_array, testSize * sizeof(int));
	int* d_adr;
	cudaGetSymbolAddress((void**)&d_adr, d_array);

	searchFrom<<<block, grid>>>(d_adr, testSize);
	cudaDeviceSynchronize();

	int* array = (int*)malloc(testSize * sizeof(int));
	cudaMemcpyFromSymbol(array, d_array, testSize*sizeof(int));

	for (int i = 0; i < testSize; i++)
	{
		std::cout << "Array at " << i << " is " << array[i] << std::endl;
	}

	return cycles;
}