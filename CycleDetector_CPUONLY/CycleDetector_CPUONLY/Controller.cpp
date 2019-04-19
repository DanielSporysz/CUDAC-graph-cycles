#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "GraphReader.h"
#include "Controller.h"

 int main(int argc, char *argv[]) {
	 // configuration
	 std::string graphFileName = "./Resources/GraphFileTemplate.txt";
	 if (argc >= 2) {
		 graphFileName = argv[1];
	 }

	 std::cout << "Reading from a file: " << graphFileName << std::endl;

	 // reading
	 std::vector<std::vector<int>> matrix = readGraphFile(graphFileName);
	 printMatrix(matrix);

	 system("pause");
	 return 0;
}