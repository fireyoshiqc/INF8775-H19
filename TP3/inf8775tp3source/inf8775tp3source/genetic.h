#pragma once
#include <vector>
#include "problem.h"
#include "solution.h"

struct GeneticIndividual {
	std::vector<int> modelGenes;
};

struct GeneticPopulation {
	std::vector<GeneticIndividual> individuals;
};

GeneticIndividual generateRandomIndividual(Problem& problem);

GeneticPopulation generatePopulation(Problem& problem);
