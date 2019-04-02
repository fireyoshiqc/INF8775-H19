#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <numeric>
#include <iostream>


/*
struct Piece {
	uint32_t amount = 0;
	uint32_t individualPrice = 0;
	uint32_t totalPrice() { return amount * individualPrice; }
	void print() {
		std::cout << "Amount: " << amount << "\tIndividual price: " << individualPrice << std::endl;
	}
};

struct Model {
	uint32_t modelIdx;
	std::vector<Piece> pieces;
	uint32_t totalCost() {
		return std::accumulate(pieces.begin(), pieces.end(), 0, [](uint32_t sum, Piece piece) { return sum + piece.totalPrice(); });
	};
	void print() {
		uint32_t index = 0;
		for (auto&& piece : pieces)
		{
			std::cout << "Piece " << index << " - ";
			piece.print();
			index++;
		}
	}
	Model& operator-=(const Model& rhs) {
		for (size_t i = 0; i < pieces.size(); i++)
			pieces[i].amount -= rhs.pieces[i].amount;
	}
};
*/

struct Model {
	size_t id;
	std::vector<int> modelCosts;
};

struct Problem {
	int nPieces; // number of pieces
	int nModels; // number of models

	std::vector<int> availablePieceBudget;
	std::vector<Model> models;
	/*
	std::vector<Piece> availablePieces;
	std::vector<Model> models;
	void printPieces() {
		for (auto&& piece : availablePieces)
			piece.print();
	}
	void printModels() {
		uint32_t index = 0;
		for (auto&& model : models)
		{
			std::cout << "Model " << index << " - ";
			model.print();
			std::cout << "-------" << std::endl;
			index++;
		}
	}
	*/
};

Problem readProblem(std::string filename);
void printProblem(Problem problem);

