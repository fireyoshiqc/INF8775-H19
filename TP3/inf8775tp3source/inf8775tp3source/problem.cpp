#include "problem.h"

#include <fstream>
#include <iostream>

Problem readProblem(std::string filename) {
	Problem problem;

	std::ifstream fichier;
	fichier.open(filename);
	if (!fichier) {
		std::cerr << "Error opening file";
		return problem;
	}

	fichier >> problem.nPieces;

	problem.availablePieceBudget = std::vector<int>(problem.nPieces);

	std::vector<int> amounts(problem.nPieces);
	std::vector<int> prices(problem.nPieces);
	for (auto&& amount : amounts)
		fichier >> amount;
	for (auto&& price : prices)
		fichier >> price;
	for (size_t i = 0; i < problem.availablePieceBudget.size(); i++)
		problem.availablePieceBudget[i] = amounts[i] * prices[i];

	fichier >> problem.nModels;

	problem.models = std::vector<Model>(problem.nModels);

	for (size_t i = 0; i < problem.models.size(); i++)
	{
		problem.models[i].modelCosts = std::vector<int>(prices.size());
		for (size_t j = 0; j < prices.size(); j++)
		{
			int qty = 0;
			fichier >> qty;
			problem.models[i].modelCosts[j] = qty * prices[j];
		}
	}

	fichier.close();

	return problem;
}

void printProblem(Problem problem) {
	std::cout << "---Problem description---" << std::endl;
	std::cout << "g (nPieces): \t" << problem.nPieces << std::endl;
	std::cout << "d (nModels): \t" << problem.nModels << std::endl;
	std::cout << "All models:" << std::endl;
	//problem.printModels();
}