#pragma once
#include <vector>
#include "problem.h"
#include "solution.h"

const size_t POPULATION_SIZE = 1;
const size_t MINIMAL_MUTATION_ITERATIONS = 1;
const size_t EVOLVE_ITERATIONS = 10;

struct GeneticIndividual {
	std::vector<int> modelGenes;
	std::vector<int> budget;
	float fitness;
};

struct GeneticPopulation {
	std::vector<GeneticIndividual> individuals;
};

GeneticIndividual generateRandomIndividual(const Problem& problem);

GeneticPopulation generateRandomPopulation(const Problem& problem);

void mutateIndividual(GeneticIndividual & individual, const Problem & problem);

void switchIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t addModelIdx, size_t subModelIdx);

void addIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t index);



void solveGenetic(const Problem & problem);