#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <numeric>
#include <iostream>

struct Model {
	size_t id;
	std::vector<int> modelCosts;
};

struct Problem {
	int nPieces; // number of pieces
	int nModels; // number of models

	std::vector<int> availablePieceBudget;
	std::vector<Model> models;
};

Problem readProblem(std::string filename);
void printProblem(Problem problem);

