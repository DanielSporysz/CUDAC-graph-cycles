#pragma once
#include <string>
#include <vector>
#include <list>

struct config_t {
	std::string fileName;
	int verticesCount;
};

enum markings
{
	connected = 1,
	disconnected = 0
};

int main(int argc, char * argv[]);
config_t readInputArguments(int argc, char *argv[]);