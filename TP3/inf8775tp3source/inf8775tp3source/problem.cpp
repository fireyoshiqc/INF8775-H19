#include "problem.h"

#include <fstream>
#include <iostream>

Problem readProblem(std::string filename) {
	Problem problem;

	std::ifstream fichier;
	fichier.open(filename);
	if (!fichier) {
		std::cerr << "Error opening file";
	}
	unsigned int i;
	fichier >> problem.nPieces;
	for (i = 0; i < problem.nPieces; i++) {
		int piece;
		fichier >> piece;
		problem.pieces.push_back(piece);
	}
	for (i = 0; i < problem.nPieces; i++) {
		int price;
		fichier >> price;
		problem.prices.push_back(price);
	}
	fichier >> problem.nModels;
	for (int modelLine = 0; modelLine < problem.nPieces; modelLine++) {
		std::vector<int> model;
		for (i = 0; i < problem.nPieces; i++) {
			int piece;
			fichier >> piece;
			model.push_back(piece);
		}
		problem.models.push_back(model);
	}
	fichier.close();

	return problem;
}

void printProblem(Problem problem) {
	std::cout << "---Problem description---" << std::endl;
	std::cout << "g (nPieces): \t" << problem.nPieces << std::endl;
	std::cout << "d (nModels): \t" << problem.nModels << std::endl;
	std::cout << "pieces: ";
	for (int i = 0; i < problem.nPieces; i++) {
		std::cout << problem.pieces[i] << " ";
	}
	std::cout << std::endl << "prices: ";
	for (int i = 0; i < problem.nPieces; i++) {
		std::cout << problem.prices[i] << " ";
	}
	std::cout << std::endl << "models: " << std::endl;
	for (int i = 0; i < problem.nModels; i++) {
		std::cout << "M" << i << ": \t";
		for (int j = 0; j < problem.nPieces; j++) {
			std::cout << problem.models[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "---         ---" << std::endl;
}