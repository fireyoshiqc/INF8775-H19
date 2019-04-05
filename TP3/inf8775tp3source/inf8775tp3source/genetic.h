#pragma once
#include <vector>
#include "problem.h"
#include "solution.h"

const int POPULATION_SIZE = 100;
const int MINIMAL_MUTATION_ITERATIONS = 3;
const int MINIMAL_ATLER_ITERATIONS = 5;
const int EVOLVE_ITERATIONS = 1000;
const int NUMBER_OF_SURVIVORS = 5; // Sandwiched by: 0 < NUMBER_OF_SURVIVOR <= POPULATION_SIZE

struct GeneticIndividual {
	std::vector<int> modelGenes;
	std::vector<int> budget;
	int totalCost;
};

// code taken from https://www.techiedelight.com/get-slice-sub-vector-from-vector-cpp/
template<typename T>
std::vector<T> slice(std::vector<T> &v, int m, int n)
{
	std::vector<T> vec(n - m + 1);
	std::copy(v.begin() + m, v.begin() + n + 1, vec.begin());
	return vec;
}

struct GeneticPopulation {
	std::vector<GeneticIndividual> individuals;
};

GeneticIndividual generateRandomIndividual(const Problem& problem);

GeneticPopulation generateRandomPopulation(const Problem& problem);

void mutateIndividual(GeneticIndividual & individual, const Problem & problem);

void switchIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t addModelIdx, size_t subModelIdx);

void addIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t index);

void sortPopulation(GeneticPopulation & population);

std::vector<GeneticIndividual> selectSurvivors(GeneticPopulation & population);

void genocide(GeneticPopulation & population, std::vector<GeneticIndividual> survivors, const Problem & problem);

void printIndividual(GeneticIndividual ind);

void evolve(GeneticPopulation & population, const Problem & problem);

void solveGenetic(const Problem & problem);