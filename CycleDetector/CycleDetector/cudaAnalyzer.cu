#pragma once
#include"cudaAnalyzer.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "vector"
#include "list"
#include <iostream>

struct cycles_t {
	int* paths;
	int* pathSizes;
	int pathsCount;
};

__global__ void searchFrom();

std::list<std::vector<int>> findCycles(int* matrix, config_t config) {
	std::list<std::vector<int>> cycles;

	dim3 block(8, 8, 8);
	dim3 grid(16,16);

	searchFrom<<<block, grid>>>();

	return cycles;
}

__global__ void searchFrom() {
	int threadsPerBlock = blockDim.x * blockDim.y * blockDim.z;
	int threadPosInBlock = threadIdx.x +
		blockDim.x * threadIdx.y +
		blockDim.x * blockDim.y * threadIdx.z;
	int blockPosInGrid = blockIdx.x +
		gridDim.x * blockDim.y +
		gridDim.x * gridDim.y * blockIdx.z;
	int tid = blockPosInGrid * threadsPerBlock + threadPosInBlock;

	// TODO
}