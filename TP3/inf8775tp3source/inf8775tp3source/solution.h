#pragma once
#include <vector>
#include "problem.h"
	
struct Solution {
	std::vector<Model> models;
	int cost;
};

void printSolution(const Solution & solution);

/*
	Function that gets the vector of remaining pieces from a solution
*/
std::vector<int> getRemainingPieces(const Problem & problem, const Solution & solution);

/*
	Function that determines if a solution is valid or not (kinda O(n²))
*/
bool isSolutionValid(const Problem & problem, const Solution & solution);

/*
	Function that looks for a type of piece in a vector of remaining pieces that is still positive
*/
bool isRemainingPiecesValid(const std::vector<int> & remainingPieces);

/*
	Function that computes the cost of a solution
*/
int getCost(Problem problem, Solution solution);