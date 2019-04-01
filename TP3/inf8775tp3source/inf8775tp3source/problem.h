#pragma once
#include <vector>
#include <string>

struct Problem {
	int nPieces; // number of pieces
	int nModels; // number of models
	std::vector<int> prices;
	std::vector<int> pieces;
	std::vector<std::vector<int>> models;
};

Problem readProblem(std::string filename);
void printProblem(Problem problem);
