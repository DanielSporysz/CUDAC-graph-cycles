#pragma once

struct config_t {
	std::string fileName;
	int matrixSize;
};

enum markings
{
	connected = 1,
	disconnected = 0
};

int main(int argc, char * argv[]);