#include "solution.h"

void printSolution(const Solution & solution) {
	for (int i = 0; i < solution.models.size(); i++) {
		std::cout << "M" << i << ": \t";
		for (int j = 0; j < solution.models[i].size(); j++) {
			std::cout << solution.models[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

/*
	Function that gets the vector of remaining pieces from a solution
*/
std::vector<int> getRemainingPieces(const Problem & problem, const Solution & solution) {
	std::vector<int> remainingPieces = problem.pieces;
	for (int i = 0; i < solution.models.size(); i++) {
		for(int j = 0; j < problem.nModels; i++) {
			remainingPieces[i] -= solution.models[j][i];
		}
	}
	return remainingPieces;
}

/*
	Function that determines if a solution is valid or not (kinda O(n²))
*/
bool isSolutionValid(const Problem & problem, const Solution & solution) {
	return isRemainingPiecesValid(getRemainingPieces(problem, solution));
}

/*
	Function that looks for a type of piece in a vector of remaining pieces that is still positive
*/
bool isRemainingPiecesValid(const std::vector<int> & remainingPieces) {
	for(int i =0; i < remainingPieces.size(); i++) {
		if(remainingPieces[i] < 0) {
			return false;
		}
	}
	return true;
}

/*
	Function that computes the cost of a solution
*/
int getCost(Problem problem, Solution solution) {
	int cost = 0;
	std::vector<int> remainingPieces = getRemainingPieces(problem, solution);
	for(int i =0; i < remainingPieces.size(); i++) {
		cost += (-1) * remainingPieces[i] * problem.prices[i];
	}
	return cost;
}