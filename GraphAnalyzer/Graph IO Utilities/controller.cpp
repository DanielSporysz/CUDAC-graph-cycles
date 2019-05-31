#pragma once
#include "GraphReader.h"

int main() {
	// Configuration
	std::string defaultFile = "./Resources/GraphFileTemplate.txt";
	
	// Reading
	config_t *config = readGraphFile(defaultFile);

	// Output
	printMatrix(config);

	// Clean up && exit
	delete config->fileName;
	free(config);

	system("pause");
	return 0;
}
