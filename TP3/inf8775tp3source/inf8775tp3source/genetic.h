#pragma once
#include <vector>
#include "problem.h"
#include "solution.h"

//TODO: find the best possible parameters (trial and error / script to run the program using 
// different parameters (would need restructuration of code to be able to pass parameters
// as non-constant values )

const int POPULATION_SIZE = 30000;
const int PROPORTION_PRESERVED_GENES_CROSSOVER = 60; //0-100 (percentage)
const int MINIMAL_MUTATION_ITERATIONS = 2;
const int MINIMAL_ATLER_ITERATIONS = 2;
const int MINIMAL_REGRESS_ITERATIONS = 2;
const int EVOLVE_ITERATIONS = 500;
const int NUMBER_OF_SURVIVORS = 28000; // Sandwiched by: 0 < NUMBER_OF_SURVIVOR <= POPULATION_SIZE
const int RANDOM_GENERATION_FILL_THRESHOLD = 10; // max number of pieces remaining for each type before stopping random filling in individual generation

struct GeneticIndividual 
{
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

struct GeneticPopulation 
{
	std::vector<GeneticIndividual> individuals;
};

std::vector<int> findBestModelsPerPieceType(const Problem & problem);

void greedyFill(GeneticIndividual & individual, const Problem & problem, std::vector<int> & bestModelsPerPieceType);

void randomFill(GeneticIndividual & individual, const Problem & problem);

GeneticIndividual generateRandomIndividual(const Problem& problem);

GeneticPopulation generateRandomPopulation(const Problem& problem);

void mutateIndividual(GeneticIndividual & individual, const Problem & problem, std::vector<int> & bestModelsPerPieceType);

void switchIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t addModelIdx, size_t subModelIdx);

void addIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t index);

GeneticIndividual crossoverMin(const GeneticIndividual & individual_a, const GeneticIndividual & individual_b, const Problem & problem, std::vector<int> & bestModelsPerPieceType);

bool compareByCost(const GeneticIndividual &a, const GeneticIndividual &b);

void sortPopulation(GeneticPopulation & population);

std::vector<GeneticIndividual> selectSurvivors(GeneticPopulation & population);

void nuclearCataclysm(GeneticPopulation & population, std::vector<GeneticIndividual> survivors, const Problem & problem, std::vector<int> & bestModelsPerPieceType);

void printIndividual(GeneticIndividual ind);

void evolve(GeneticPopulation & population, const Problem & problem, std::vector<int> & bestModelsPerPieceType);

void solveGenetic(const Problem & problem, int nIterations = EVOLVE_ITERATIONS);